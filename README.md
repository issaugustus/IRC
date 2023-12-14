# ft_irc

## Clients
### Rôle :
Ce sont les utilisateurs du serveur, ils peuvent envoyer et recevoir des messages
### Attributs : (On doit implementer la classe qui decrira chaque client)
- nickname : string de 9 caracteres max
- hostname (ip ou bien nom de domaine ?)
- username
- ip du serveur auquel il est connecté
### Fonctions : (on ne doit pas les coder car c'est la partie client) 
- Connexion au serveur
- Definir un nickname
- Definir un username
- Rejoindre un canal
- Envoyer et recevoir des messages privés aux autres clients du serveur
- Envoyer et recevoir des messages a/provenant d'un canal

## Operateurs de canal: (chanop)
### Rôle :
Ce sont des clients (donc ils heritent de la classe client) qui ont des permissions supplementaires sur un canal
### Attributs : (On doit implementer la classe derivee qui decrira les clients qui sont des operateurs de canal)

### Fonctions : (on ne doit pas les coder car c'est la partie client)
- KICK : virer un client du canal
- INVITE : inviter un client dans le canal
- TOPIC : modifier ou voir le sujet/titre d'un canal
- MODE : modifier le mode d'un canal
    - i : activer/desactiver le flag invite only (le canal ne peut etre rejoint que sur invitation)
    - t : activer/desactiver la modification du topic par les non-operateurs
    - k : definir ou supprimer le mot de passe d'un canal
    - o : donner ou supprimer les privileges d'operateurs a un client
    - l : definir ou supprimer la limite du nombre d'utilisateurs d'un canal

## Operateurs de serveur:
## Rôle :
Ce sont des clients qui ont des permissions supplementaires sur l'ensemble du serveur et donc heritent des chanop. On devient operateur de serveur avec la commande /oper avec la condition que les attributs du client soient dans la config du serveur
### Attributs : (classe derivee de chanop)

### Fonctions:
- KILL : deconnecter de force un client
- BAN : bannir un client du serveur

===============================================================================================================================================
Bien sûr ! Voici les points clés pour gérer un serveur IRC en C++ sans développer de client et sans gérer la communication entre les serveurs. Je vais également vous expliquer comment gérer plusieurs clients en même temps en mode non bloquant :

Création du socket d'écoute : Utilisez la fonction socket pour créer un socket TCP/IP sur lequel le serveur écoutera les connexions des clients IRC.

Liaison du socket d'écoute : Utilisez la fonction bind pour lier le socket d'écoute à une adresse IP et un port spécifiques sur le serveur. Cela permettra aux clients de se connecter à cette adresse et à ce port.

Mise en écoute : Utilisez la fonction listen pour mettre le socket d'écoute en mode d'écoute, permettant au serveur d'accepter les connexions entrantes des clients.

Gestion des clients en mode non bloquant : Utilisez des opérations non bloquantes pour gérer plusieurs clients en même temps. Voici une approche possible :

a. Utilisez la fonction fcntl avec l'option F_SETFL pour mettre le socket d'écoute en mode non bloquant. Cela permettra de ne pas bloquer le processus principal lorsqu'il attend une nouvelle connexion.

b. Créez une structure de données pour stocker les informations sur les clients connectés, telle qu'une liste chaînée ou un tableau dynamique. Cette structure devrait contenir le descripteur de fichier du socket de chaque client, ainsi que d'autres informations pertinentes.

c. Utilisez la fonction select ou poll pour surveiller à la fois le socket d'écoute et les descripteurs de fichiers des clients connectés. Cela vous permettra de détecter les événements d'entrée/sortie (lecture/écriture) sur ces descripteurs.

d. Lorsque select ou poll indique qu'un descripteur de fichier a des données disponibles en lecture, vous pouvez utiliser la fonction accept pour accepter une nouvelle connexion entrante. Ajoutez ensuite le nouveau client à votre structure de données.

e. Lorsque select ou poll indique qu'un descripteur de fichier a des données disponibles en lecture ou en écriture, vous pouvez lire ou écrire des données à partir de ce descripteur pour communiquer avec le client correspondant.

f. Assurez-vous d'utiliser des opérations non bloquantes pour les lectures et les écritures, afin de ne pas bloquer le processus principal lorsqu'un client est inactif.

Gestion des commandes : Lorsqu'un client envoie des commandes IRC, vous devez les traiter en fonction de leur type. Analysez les commandes reçues et implémentez les fonctionnalités correspondantes du serveur IRC, telles que la gestion des canaux, des messages privés, etc.

Gestion des déconnexions : Lorsqu'un client se déconnecte, assurez-vous de nettoyer toutes les ressources associées à cette connexion, telles que fermer le descripteur de fichier correspondant et le retirer de votre structure de données.

Assurez-vous de gérer les erreurs potentielles tout au long du processus et d'implémenter une boucle principale qui gère les connexions entrantes, les commandes des clients et les opérations d'entrée/sortie de manière efficace.

La gestion des clients en mode non bloquant permet de traiter plusieurs connexions simultanément sans bloquer le processus principal, garantissant ainsi une gestion efficace des clients IRC.
