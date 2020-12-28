# Level 03

Lorsqu'on exécute le binaire un password est attendu sur l'entrée standard.

Dans un premier temps, on peut réécrire la fonction _main_ en C, de la sorte :

```c
int main(int argc, const char **argv)
{
	int a; // esp+0x1c
	srand(time(NULL));

	puts("***********************************");
	puts("*\t\tlevel03\t\t**");
	puts("***********************************");
	printf("Password:");
	scanf("%d", &a);
	test(a, 322424845);
	return (0);
}
```

Ensuite la fonction _test_ est un peu plus complexe. Regardons le code asm :

```asm
   0x08048747 <+0>:     push   ebp
   0x08048748 <+1>:     mov    ebp,esp
   0x0804874a <+3>:     sub    esp,0x28

   0x0804874d <+6>:     mov    eax,DWORD PTR [ebp+0x8]
   0x08048750 <+9>:     mov    edx,DWORD PTR [ebp+0xc]
   0x08048753 <+12>:    mov    ecx,edx
   0x08048755 <+14>:    sub    ecx,eax
   0x08048757 <+16>:    mov    eax,ecx
   0x08048759 <+18>:    mov    DWORD PTR [ebp-0xc],eax
   0x0804875c <+21>:    cmp    DWORD PTR [ebp-0xc],0x15
   0x08048760 <+25>:    ja     0x804884a <test+259>

   0x08048766 <+31>:    mov    eax,DWORD PTR [ebp-0xc]
   0x08048769 <+34>:    shl    eax,0x2
   0x0804876c <+37>:    add    eax,0x80489f0
   0x08048771 <+42>:    mov    eax,DWORD PTR [eax]
   0x08048773 <+44>:    jmp    eax

   0x08048775 <+46>:    mov    eax,DWORD PTR [ebp-0xc]
   0x08048778 <+49>:    mov    DWORD PTR [esp],eax
   0x0804877b <+52>:    call   0x8048660 <decrypt>
   0x08048780 <+57>:    jmp    0x8048858 <test+273>

   0x08048785 <+62>:    mov    eax,DWORD PTR [ebp-0xc]
   0x08048788 <+65>:    mov    DWORD PTR [esp],eax
   0x0804878b <+68>:    call   0x8048660 <decrypt>
   0x08048790 <+73>:    jmp    0x8048858 <test+273>

   0x08048795 <+78>:    mov    eax,DWORD PTR [ebp-0xc]
   0x08048798 <+81>:    mov    DWORD PTR [esp],eax
   0x0804879b <+84>:    call   0x8048660 <decrypt>
   0x080487a0 <+89>:    jmp    0x8048858 <test+273>

   0x080487a5 <+94>:    mov    eax,DWORD PTR [ebp-0xc]
   0x080487a8 <+97>:    mov    DWORD PTR [esp],eax
   0x080487ab <+100>:   call   0x8048660 <decrypt>
   0x080487b0 <+105>:   jmp    0x8048858 <test+273>

   0x080487b5 <+110>:   mov    eax,DWORD PTR [ebp-0xc]
   0x080487b8 <+113>:   mov    DWORD PTR [esp],eax
   0x080487bb <+116>:   call   0x8048660 <decrypt>
   0x080487c0 <+121>:   jmp    0x8048858 <test+273>

   0x080487c5 <+126>:   mov    eax,DWORD PTR [ebp-0xc]
   0x080487c8 <+129>:   mov    DWORD PTR [esp],eax
   0x080487cb <+132>:   call   0x8048660 <decrypt>
   0x080487d0 <+137>:   jmp    0x8048858 <test+273>

   0x080487d5 <+142>:   mov    eax,DWORD PTR [ebp-0xc]
   0x080487d8 <+145>:   mov    DWORD PTR [esp],eax
   0x080487db <+148>:   call   0x8048660 <decrypt>
   0x080487e0 <+153>:   jmp    0x8048858 <test+273>

   0x080487e2 <+155>:   mov    eax,DWORD PTR [ebp-0xc]
   0x080487e5 <+158>:   mov    DWORD PTR [esp],eax
   0x080487e8 <+161>:   call   0x8048660 <decrypt>
   0x080487ed <+166>:   jmp    0x8048858 <test+273>

   0x080487ef <+168>:   mov    eax,DWORD PTR [ebp-0xc]
   0x080487f2 <+171>:   mov    DWORD PTR [esp],eax
   0x080487f5 <+174>:   call   0x8048660 <decrypt>
   0x080487fa <+179>:   jmp    0x8048858 <test+273>

   0x080487fc <+181>:   mov    eax,DWORD PTR [ebp-0xc]
   0x080487ff <+184>:   mov    DWORD PTR [esp],eax
   0x08048802 <+187>:   call   0x8048660 <decrypt>
   0x08048807 <+192>:   jmp    0x8048858 <test+273>

   0x08048809 <+194>:   mov    eax,DWORD PTR [ebp-0xc]
   0x0804880c <+197>:   mov    DWORD PTR [esp],eax
   0x0804880f <+200>:   call   0x8048660 <decrypt>
   0x08048814 <+205>:   jmp    0x8048858 <test+273>

   0x08048816 <+207>:   mov    eax,DWORD PTR [ebp-0xc]
   0x08048819 <+210>:   mov    DWORD PTR [esp],eax
   0x0804881c <+213>:   call   0x8048660 <decrypt>
   0x08048821 <+218>:   jmp    0x8048858 <test+273>

   0x08048823 <+220>:   mov    eax,DWORD PTR [ebp-0xc]
   0x08048826 <+223>:   mov    DWORD PTR [esp],eax
   0x08048829 <+226>:   call   0x8048660 <decrypt>
   0x0804882e <+231>:   jmp    0x8048858 <test+273>

   0x08048830 <+233>:   mov    eax,DWORD PTR [ebp-0xc]
   0x08048833 <+236>:   mov    DWORD PTR [esp],eax
   0x08048836 <+239>:   call   0x8048660 <decrypt>
   0x0804883b <+244>:   jmp    0x8048858 <test+273>

   0x0804883d <+246>:   mov    eax,DWORD PTR [ebp-0xc]
   0x08048840 <+249>:   mov    DWORD PTR [esp],eax
   0x08048843 <+252>:   call   0x8048660 <decrypt>
   0x08048848 <+257>:   jmp    0x8048858 <test+273>

   0x0804884a <+259>:   call   0x8048520 <rand@plt>
   0x0804884f <+264>:   mov    DWORD PTR [esp],eax
   0x08048852 <+267>:   call   0x8048660 <decrypt>
   0x08048857 <+272>:   nop
   0x08048858 <+273>:   leave
   0x08048859 <+274>:   ret
```

De la ligne +6 à la ligne +25, une comparaison est faite sur la différence des deux arguments de la fonction et le nombre 0x15 (22 dec). Si la différence est supérieure à 21, on jump vers la fin de la fonction, à l'adresse `0x804884a`.
En C :

```c
void test(int a, int b)
{
	int diff = b - a; // ebp - 0xc

	if (diff > 21)
	{
		decrypt(rand());
		return;
	}
}
```

De la ligne +31 à la ligne +44, on jump selon l'adresse stocké à l'adresse `diff << 2 + 0x80489f0`. Allons voir ce qu'il y a à l'adresse `0x80489f0` avec gdb : `x/22wx 0x80489f0`. On retrouve 22 adresses, qui pointent toutes vers une adresse de la fonction _test_. À ces adresses on a soit la suite d'instructions :

```asm
mov    eax,DWORD PTR [ebp-0xc]
mov    DWORD PTR [esp],eax
call   0x8048660 <decrypt>
jmp    0x8048858 <test+273>
```

qui permet d'appeler la fonction _decrypt_ ayant un argument étant la différence des deux arguments de la fonction _test_.

Soit la fin de la fonction _test_ :

```asm
call   0x8048520 <rand@plt>
mov    DWORD PTR [esp],eax
call   0x8048660 <decrypt>
```

qui fera appel à la fonction _decrypt_ avec un nombre aléatoire.

On obtient le code suivant en C :

```c
void test(int a, int b)
{
	int diff = b - a; // ebp - 0xc

	if (diff > 21)
	{
		decrypt(rand());
		return;
	}
	else // jump to adress pointed by : diff << 2 + 0x80489f0
	{
		if (diff == 0 || (diff >= 10 && diff <= 15))
			decrypt(rand());
		else
			decrypt(diff);
	}
}
```

On fait donc seulement appel à la fonction _decrypt_ lorsque la différence est comprise entre 1 et 9 ou 16 et 21.

En testant ces valeurs on se rend compte qu'un shell s'ouvre lorsque la différence vaut 18.

```
(echo "322424827"; cat) | ./level03
```
