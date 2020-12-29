# Level 06

Lorsqu'on exécute le binaire, un login et un serial sont attendus sur l'entrée standard.

On peut réécrire un code source en C à partir du code asm du binaire :

```c
int auth(char *login, uint serial)
{
	uint len; // ebp-0xc
	uint res; // ebp-0x10
	uint i;	  // ebp-0x14

	login[strcspn(login, "\n")] = 0;

	len = strnlen(login, 0x20);

	if (len <= 5)
		return 1;

	if (ptrace(PTRACE_TRACEME, 0, 1, 0) == 0xffffffff)
	{
		puts("\033[32m.---------------------------.");
		puts("\033[31m| !! TAMPERING DETECTED !!  |");
		puts("\033[32m'---------------------------'");
		return 1;
	}

	res = (login[3] ^ 0x1337) + 0x5eeded;
	i = 0;
	while (i < len)
	{
		if (login[i] <= 0x1f)
			return 1;

		int c = login[i] ^ res;
		int nb = 0x88233b2b * c;
		int nb2 = c - nb;

		nb2 >> 1;
		nb2 += nb;
		nb2 >> 10;
		nb2 *= 0x539;
		res = res + c - nb2;
		i++;
	}

	if (res == serial)
		return 0;
	return 1;
}

int main(int argc, const char **argv)
{
	char *login; // esp+0x2c
	uint serial; // esp+0x28

	puts("***********************************");
	puts("*\t\tlevel06\t\t  *");
	puts("***********************************");
	printf("-> Enter Login: ");
	fgets(login, 0x20, stdin);

	puts("***********************************");
	puts("***** NEW ACCOUNT DETECTED ********");
	puts("***********************************");
	printf("-> Enter Serial: ");

	scanf("%u", &serial);

	if (!auth(login, serial))
	{
		puts("Authenticated!");
		system("/bin/sh");
		return 0;
	}
	return 1;
}
```

On a affaire ici à un système de connexion. Si on fournit un bon couple login / serial on peut accéder à un shell.

Seulement si la fonction _auth_ retourne 0 un shell est lancé. En regardant dans la fonction _auth_, on se rend compte que c'est le cas lorsque `res = serial`. Si on met un breakpoint au niveau de cette comparaison, on peut regarder la valeur à laquelle `serial` doit être mise pour passer la condition.

De plus il faut que la longueur du login soit strictement supérieure à 5 caractères pour ne pas quitter la fonction directement.

En faisant cela, on a le message :

```
.---------------------------.
| !! TAMPERING DETECTED !!  |
'---------------------------'
```

dû à un appel à la fonction _ptrace_.
Il faut donc modifier la valeur de retour de cette fonction pour pouvoir s'arrêter au breakpoint suivant. Il n'y a plus qu'à regarder la valeur contenu à l'adresse `ebp-0x10`, la variable qui nous intéresse.

Avec gdb :

```gdb
b *0x080487ba
b *0x08048866
r
*login d'au moins 6 caractères*
*serial de votre choix*
set $eax = 1
c
x/wd $ebp-0x10
```

On a maintenant le login et le serial, on peut donc maintenant les utiliser pour se connecter.

## Commandes

```bash
(echo "abcdef" ; echo "6232802" ; cat) | ./level06
```
