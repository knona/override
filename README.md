# Override

## Présentation du projet

Override est un projet d'exploitation de binaires et de reverse engineering. Il va falloir user de différentes failles, notamment de **buffer overflow** et de **format string attack**.

Le projet se fait avec une machine virtuelle. Les utilisateurs s'appelent tous `levelXX` où `XX` est le numéro du level. Vous pouvez vous vous connecter au premier utilisateur avec le couple login / mot de passe suivant : `level00:level00`. Tous les utilisateurs possèdent un fichier `.pass` dans leur _home_ contenant leur mot de passe. Le but est donc d'exploiter le binaire afin d'obtenir le mot de passe du prochain level.

Vous pouvez retrouver l'iso à cette [adresse](https://drive.google.com/file/d/16SxL1ygQCOMdL_OBOiPmDZlxO0IdfB9w/view?usp=sharing).

Dans ce repo vous trouverez différents dossiers correspondant aux levels et contenant une explication pour procéder à l'exploitation du binaire.

## Paquets utiles

### sshpass

Ce paquet est optionnel mais permet l'utilisation de mot de passe ssh directement depuis la ligne de commande.

**Linux** :

- `sudo apt install sshpass`

**MacOs** :

- `brew install hudochenkov/sshpass/sshpass`
  or
- `brew install https://raw.githubusercontent.com/kadwanev/bigboybrew/master/Library/Formula/sshpass.rb`

### Peda

Peda permet de générer un fichier `.gdbinit` qui embellira et améliora l'utilisation de **gdb**. Il peut s'avérer très pratique.

- clone le projet : `git clone https://github.com/longld/peda.git peda`
- l'envoyer sur la vm : `sshpass -p 'level00' scp -P 4242 -r peda level00@localhost:/tmp`
- sur la vm exécuter la commande : `chmod 777 ~/ && echo "source /tmp/peda/peda.py" >> ~/.gdbinit`
