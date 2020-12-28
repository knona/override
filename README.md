# Override

## sshpass

Ce paquet est optionnel mais permet l'utilisation de mot de passe ssh directement depuis la ligne de commande.

**Linux** :

- `sudo apt install sshpass`

**MacOs** :

- `brew install hudochenkov/sshpass/sshpass`
  or
- `brew install https://raw.githubusercontent.com/kadwanev/bigboybrew/master/Library/Formula/sshpass.rb`

## Peda

- clone le projet : `git clone https://github.com/longld/peda.git peda`
- l'envoyer sur la vm : `sshpass -p 'level00' scp -P 4242 -r peda level00@localhost:/tmp`
- sur la vm exécuter la commande : `chmod 777 ~/ && echo "source /tmp/peda/peda.py" >> ~/.gdbinit`
