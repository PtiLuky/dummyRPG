# Fonctionnalités voulues pour dummyRPG

## Jeu

* 65'535 (uint16) cartes max
* 65'535 (uint16) objets max
* 4'294'967'295 (uint32) characters max (alliés + pnj + créatures)
* 4'294'967'295 (uint32) événements max
* 255 (uint8) palettes (de 4064*4064 max)

## Cartes

* parentage de maps possible 
* Une carte contient : 
  * 255 (uint8) étages max. Chaque étage est un étage d'interactivité avec le joueur. Quand le joueur est sur un étage, seulement cet étage et les étages inférieurs sont montrés à l'écran.
  * 8 palettes max
* Un étage contient :
  * *des zones d'apparition*
  * *des événements*
  * *des pnj*
  * une couche de blocage (blocking layer) : où le joueur peut marcher ou non
  * 2 couches graphiques inférieures (sous le joueur)
  * 6 couches graphiques max (2 par défaut) supérieures (devant le joueur)
* Une couche graphique contient :
  * 2'000 lignes max ~~65'535 (uint16) lignes max~~
  * 2'000 colonnes max ~~65'535 (uint16) colonnes max~~
  * x (uint8) colonne de la tuile sur la palette originale
  * y (uint8) ligne de la tuile sur la palette originale
  * id (0 <= id < 8) de la palette originale dans la map
* La palette originale :
  * 254 (uint8 - 1) tuiles max par ligne/colonnes **(= 4064 MAX sur la largeur et la hauteur des palette pour des tuiles de 16px !!!)** 
 
**Taille d'une carte en RAM :**  
Par tuile : 3 * uint8 = 3 bytes  
Nb Tuiles max : 65'535 * 65'535 = 4'294'836'225  (sans restriction à 2'000 lignes/colonnes)  
Taille par étages : 4'294'836'225 * (8 * 3 (visibles) + 1 (blocking)) = 107'370'905'625‬ 
**Aux tailles maximums un étage fait 107 Go...**

Avec la restriction à 2000, taille max par étage : 100Mo
  
## Zones d'apparition
* rectangle de la carte
* liste de types de monstres
  * taux d'apparition par level de monstre
* nombre max de monstres dans la zone 
* Indice de capacité de régénération de la zone. Les valeurs et leurs significations sont encore à établir. L'idée est que si cet indice est bas, les monstres de la zone mettent beaucoup de temps à réapparaitre.
* les (ré)apparitions sont mises en pauses lorsque la carte n'est pas la carte active (une autre est chargée).
* lorsque la carte de la zone re-devient active la zone doit contenir les memes monstres que lorsque le joueur a quitté
* au tout premier chargement de la zone, celle-ci doit contenir un nombre N de monstres. Avec N = nombre max / 2.

## Combats

* Combat perso vs monstres
* on ne controle pas de créature (mode pokemon)
* drop/loot à la fin du combat

## Monstres

* différents types de monstres
* chaque type : stats et drops et couleur (?) en fonction du niveau
* chaque monstre a un type + niveau
* Niveaux allant de 1 à 100

## Drops

* A la fin de chaque combat
* les taux de drop dépendent du type + niveau du monstre tué

## Crafting

* Assemblage d'objets type A + B = C et C + C = super C

## Economie

* pieces bronze argent or

## Farming 

* ?

## Events

* activé par une action (arrivée sur une case, interaction avec un pnj, objet...)
* un Event peut :
  * changer de map
  * changer de floor
  * lire/écrire une variable (hasKilledBoss, hasTalkedToXXX, ...)
  * regarder le nombre/supprimer/ajouter des ressources (objet, monnaie, ...)
  * déplacer un personnage
  * changer les stats d'un personnage
  * changer un layer graphique (?)
  * changer un layer blocking (?)
  * changer les propriétés d'une zone d'apparition (?)
* le joueur n'a accès à aucun contrôle durant les events


## Dialogues

* système de clé-valeur + un fichier qui référence tout le texte => permet une traduction facile
* possibilité de choisir sa réponse (peut influencer le dialogue/l'histoire)
* possibilité de lire/écrire une variable (hasKilledBoss, chosenTheWisePath, ...)
