# Level 01

Lorsqu'on exécute le binaire un username est demandé sur l'entrée standard.

En désassemblant le binaire, on a deux fonctions _verify_user_name_ et _verify_user_pass_ appelées par la fonction _main_.

Le code asm est assez classique comme vu dans le projet rainfall. Cepdendant, il y a quelques instructions un peu spéciales dans nos deux fonctions \_verify\_\_ :

```asm
mov    eax,DWORD PTR [ebp+0x8]
mov    edx,eax
mov    eax,0x80486b0
mov    ecx,0x5
mov    esi,edx
mov    edi,eax
repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
seta   dl
setb   al
mov    ecx,edx
sub    cl,al
mov    eax,ecx
movsx  eax,al
```

Ce code là est une portion du code désassemblée de la fonction _verify_user_pass_.
Le code semble long et complexe mais en réalité il effectue quelque chose de plutôt simple.

Ces premières lignes :

```asm
mov    eax,DWORD PTR [ebp+0x8]
mov    edx,eax
mov    eax,0x80486b0
mov    ecx,0x5
mov    esi,edx
mov    edi,eax
```

permettent d'initialiser les registres avant la prochaine instruction.
On aura donc à la fin de ces instructions :

- `ecx`: 0x5
- `esi`: `[ebp+0x8]` cette adresse correspond au 1er argument de la fonction, soit le mot de passe entré par l'utilisateur
- `edi`: `0x80486b0` cette adresse contient "admin"

On a donc ensuite l'instruction :

```asm
repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
```

qui va comparer les deux chaînes de caractères pointées par `esi` et `edi`. La comparaison s'arrête lorsque les caractères des deux chaînes diffèrent ou lorsque le compteur `ecx` tombe à 0.

```asm
rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
```

Les prochines instructions :

```asm
seta   dl
setb   al
mov    ecx,edx
sub    cl,al
mov    eax,ecx
movsx  eax,al
```

mettent le resgistre `eax` à 0 uniquement si les deux chaînes de caractères sont équivalentes sur la portion demandée.
Cela ressemble beaucoup à la fonction memcmp dans l'idée.

Le registre `eax` représentant la valeur retournée d'une fonction, les fonctions \_verify\_\_ renveront le résultat de cette comparaison.

A partir de ce code et à l'aide de gdb, on peut réécrire en C un code source :

```c
char a_user_name[256]; // 0x804a040

int verify_user_name(void)
{
	puts("verifying username....\n");
	return memcmp("dat_wil", a_user_name, 7); // not really a call to memcmp function
}

int verify_user_pass(char *pass)
{
	return memcmp("admin", pass, 5); // not really a call to memcmp function
}

int main(int argc, const char **argv)
{
	char buffer[64]; // esp+0x1c
	int a;			 // esp+0x5c

	int i = 0;
	while (i < 16)
	{
		int j = i * 4;
		buffer[j] = 0;
		buffer[j + 1] = 0;
		buffer[j + 2] = 0;
		buffer[j + 3] = 0;
		i++;
	}
	// same as bzero(buffer, 64);

	a = 0;

	puts("********* ADMIN LOGIN PROMPT *********");
	printf("Enter Username: ");
	fgets(a_user_name, 256, stdin);
	if (verify_user_name())
	{
		puts("nope, incorrect username...\n");
		return 1;
	}
	puts("Enter Password: ");
	a = verify_user_pass(fgets(buffer, 100, stdin));
	if (a == 0)
	{
		puts("nope, incorrect password...\n");
		return 1;
	}
	else
	{
		if (a == 0)
			return 0;
		else
		{
			puts("nope, incorrect password...\n");
			return 1;
		}
	}
}

```

On voit bien dans cette portion de code :

```c
a = verify_user_pass(fgets(buffer, 100, stdin));
if (a == 0)
{
	puts("nope, incorrect password...\n");
	return 1;
}
else
{
	if (a == 0)
		return 0;
	else
	{
		puts("nope, incorrect password...\n");
		return 1;
	}
}
```

que la vérification du mot de passe n'a pas de sens même si on saisit "admin".

On va donc devoir trouver une faille pour pouvoir insérer du shellcode.

On remarque que la fonction `fgets` utilisé pour obtenir le mot de passe, lit 100 caractères alors que le buffer en fait 64. On peut donc user d'un stack buffer overflow pour écraser l'**eip sauvegardé**.

On appele **eip sauvegardé** l'adresse contenue dans la registre eip qui a été copiée dans la stack.

Le registre `eip` contient habituellement l'adresse de la prochaine instruction à exécuté.

Avec un stack buffer overflow on peut la remplacer pour pouvoir se déplacer dans notre programme et exécuter du code que l'on veut.

On va donc injecter un shellcode et remplacer l'**eip sauvegardé** par l'adresse où se situe notre shellcode.

On peut par exemple injecter du shellcode, dans le premier argument. En effet, c'est un buffer assez gros de 256 caractères, ce qui nous laisse pas mal de place pour écrire notre shellcode.
Cependant, il faudra bien l'écrire après "dat_wil" car on doit quand même respecter la condition de l'username si on veut faire un buffer overflow sur le deuxième appel à la fonction `fgets` (qui attend le mot de passe). La comparaison se basant uniquement sur les 7 premiers caractères, cela ne pose pas problème.

### Commande

```bash
(cat args/arg1 ; cat args/arg2) | ./level01
```
