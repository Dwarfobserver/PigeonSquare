
# PigeonSquare

TP orienté sur la programmation concurrente.
Le but est de visualiser des pigeons, chacun géré depuis un thread séparé.
On peut nourrir ces pigeons ou les effrayer en cliquant sur la fenêtre.

Actuellement, l'application supporte plusieurs milliers de pigeons à un taux de rafraîchissement de 60 (environ 9000 sur
Windows 10 avec un Core i5 à 2.3GHz (4 threads simultanés)).

### Utilisation

Le TP supporte OSX et Windows, et utilise C++14.
Le build Windows se fait avec la commande "cmake .".
Le build Apple se fait avec les commandes suivantes : "brew install SFML", "cmake CMakeLists.txt" puis "make".

### Structure du programme

Il y a trois classes principales :

 - Window :

Cette classe stocke les textures, et affiche tous ses sprites triés par leur ordonnée (afin que les plus proches
apparaîssent à l'avant). Elle reçoit également les inputs de l'utilisateur, qu'elle utilise via les fonctions de World.

Les fonctions qui permettent de créer et supprimer un sprite via un handle peuvent être appelées de façon
asynchrone : les modifications sont stockées dans une queue de callbacks synchronisée (appelée TaskQueue). Après chaque
affichage, la queue est vidée de façon synchrone, en exécutant toutes les modifications.

La fonction pour modifier la position d'un sprite (appelée par les pigeons) est la plus optimisée : elle stocke de façon
thread-safe la position qui va être lue (sans lock) lors de l'actualisation des positions par l'instance Window.
Toutefois, cette méthode fait retarder d'une update la position des pigeons à l'écran.

 - World :

Cette classe répertorie tous les pigeons, elle fait office de communicateur. Les fonctions déclenchées la fenêtre (lors
d'inputs) envoient un message à chaque pigeon. Les pigeons peuvent également signaler à World lorsqu'ils ont atteint un
pain, afin d'indiquer à tous les pigeons que le pain a été mangé (s'il s'agit bien du premier appel à la fonction pour
un pain donné).

   Toutes les fonctions de World sont synchronisées sur un unique mutex (une seule s'exécute à la fois).

 - Pigeon :

Hérite de Runnable, ce qui lui confère un thred dédié et des fonctions pour le lancer ou l'arrêter de façon asynchrone.
Le thread se lance avec PigeonConfig, qui spécifie tous les paramètres du pigeon (vitesse, nombre d'updates par
secondes, portée de fuite, ...). Une seed est conféré à chaque pigeon : pas pour qu'il la mange, mais pour qu'il génère
ses propres chiffres aléatoires.

Les signaux asynchrones provenant de World sont stockés danns une TaskQueue (de la même manière que Window) afin qu'ils
soient traités après l'update de l'état du pigeon. Cet état est une fonction qui indique si l'état est fini ou non et
qui actualise le pigeon à chaque fois qu'elle est actualisée.
