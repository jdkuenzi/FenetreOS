# Fenetre_OS

## Kernel
Concernant les fonctionnalités côté kernel, nous avons tout implémenté et normalement tout fonctionne correctement sans bug.

Nous avons ajouté quelques fonctionnalités à notre OS
- Dans la partie timer
    - la génération d'un nombre random
- Dans la partie vid
    - Effacement de l'écran à partir d'une position x et y
- Dans la partie stdio
    - Ecriture d'un string avec une couleur passée en argument (utilisé pour print les messages d'erreur)
    - Ecriture d'un string à une position x et y sans mettre à jour le curseur
- Dans la partie file_system (simule un système de fichier via les modules de grub)
    - Recherche d'un fichier via son nom dans les modules
    - Récupération d'information d'un fichier (son nom et sa taille)
    - Copie du fichier dans un buffer
- Dans la partie keyboard
    - La manipulation du clavier (read_string)
- Dans la partie syscall
    - Syscall pour la mainupation du clavier
    - Syscall pour demander de sleep un certain temps
    - Syscall pour get un nombre random entre 0 et un max
    - Syscall pour print un string à un x et y
    - Syscall pour clean l'écran depuis un x et y

Nous avons aussi amélioré la façons de print le logo animé dans le timer. Avant nous métions toujours à jour la position du curseur ce qui faisait beaucoup d'appel au registre. Maintenant nous écrivons juste au x et y souhaité sans mettre à jour la position du curseur (aucun appel au registre).

## User
Concernant les fonctionnalités coté user, le shell est totalement oppérationnel et toutes les fonctionnalités sont correctement implémentées.

Concernant l'application utilisateur, nous avons fait un puissance 4 avec plusieurs mode de jeux
- Player VS Player
- Player VS Dumb IA
- Player VS Hard IA
- Hard IA VS Hard IA

La dumb IA est tout simplement l'ordinateur qui joue dans une colonne aléatoire.

La hard IA fonctionne en 3 étapes
1. l'ordinateur regarde s'il peut gagner, si oui, il joue dans la colonne qui le fait gagner
2. il regarde s'il peut empêcher le joueur de gagner, si oui, il joue dans la colonne qui bloque le joeur
3. il joue dans une colonne aléatoire s'il ne peut ni gagner ni empêcher le joueur de gagner

## Librairie

Nous avons ajouté quelques fonctionnalités dans nos librairie
- Dans la librairie string
    - La concatenation de 2 string ensemble
    - La comparaison si un string commence avec un autre
    - Le trim d'un string
    - La mise en minuscule d'un string
- Dans la librairie utils
    - La conversion d'un nombre écrit sous forme de string en int (ex: "-10" -> -10)
    - Le build d'un string à argument variable sous forme de printf (snprinft)