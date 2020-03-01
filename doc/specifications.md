# Fonctionnalités voulues pour dummyRPG

## Jeu

* 65'535 (uint16) cartes max
* 65'535 (uint16) objets max
* 4'294'967'295 (uint32) characters max (alliés + pnj + créatures)
* 4'294'967'295 (uint32) événements max
* 255 (uint8) palettes max (de 4064*4064 max)

## Cartes

* parentage de cartes possible 
* Une carte contient : 
  * 255 (uint8) étages max. Chaque étage est un étage d'interactivité avec le joueur. Quand le joueur est sur un étage, seulement cet étage et les étages inférieurs sont montrés à l'écran.
  * 4 palettes max
* Un étage contient :
  * *des zones d'apparition*
  * *des événements*
  * *des pnj*
  * une couche de blocage (blocking layer) : où le joueur peut marcher ou non
  * 2 couches graphiques inférieures (sous le joueur)
  * 6 couches graphiques supérieures max (2 par défaut) (devant le joueur)
* Une couche graphique contient :
  * 1'024 lignes max ~~65'535 (uint16) lignes max~~
  * 1'024 colonnes max ~~65'535 (uint16) colonnes max~~
  * x (uint8) colonne de la tuile sur la palette originale
  * y (uint8) ligne de la tuile sur la palette originale
  * id (0 <= id < 4) de la palette originale dans la map
* La palette originale :
  * 254 (uint8 - 1) tuiles max par ligne/colonnes **(= 4064 MAX sur la largeur et la hauteur des palette pour des tuiles de 16px !!!)** 
 
**Taille d'une carte en RAM :**  
Par tuile : 3 * uint8 = 3 bytes  
Nb Tuiles max : 65'535 * 65'535 = 4'294'836'225  (sans restriction à 1'024 lignes/colonnes)  
Taille par étages : 4'294'836'225 * (8 * 3 (visibles) + 1 (blocking)) = 107'370'905'625‬  
**Aux tailles maximales un étage fait 107 Go...**

Avec la restriction à 1024, taille max par étage : 26Mo
  
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
* on ne controle pas de créature (contrairement à pokemon)
* drop/loot à la fin du combat

## Monstres

* différents types de monstres
* chaque monstre a un type + niveau
* proposition: les monstres de même type on une couleur differente ne fonction de leur niveau pour faire une differenciation visuelle des differents niveaux. exemple slime lvl1-20 = vert etc... 
* Niveaux allant de 1 à 100

## Drops

* A la fin de chaque combat si combat victorieux
* les taux de drop dépendent du type + niveau du monstre tué
* items droped en fonction du type de monstre

## Metiers

  Forgeronerie:
	* Le personnage a la capacité de forger des objets a partir du loot récupéré sur les monstres, acheté, collecté, recupéré durant les quetes (très similaire a Skyrim)
	* Les items craftés en forgeronerie sont des equipements (armes, protections, anneaux etc...)
	* Il faut une forge pour forger
	* Assemblage d'objets type A + B = C
	* proposition: systeme de probabilité de réussite ou de propabilité sur les stats? pas fan
	* proposition: recettes a collecter pour savoir forget un element? Formation au pres du forgeron des differents villages?
	* Le prix de vent des items craftés est plus elevé que la somme des prix de vente des elements utilisés pour crafter l'item

  Alchimie:
	* Le personnage a la capacité de créer des items aux effets differents a partir du loot récupéré sur les monstres, acheté, collecté, recupéré durant les quetes.
	* Les items craftés en Alchimie sont des potions (potion de vie, de mana, etc...) 
	* il faut un laboratoire d'alchimie pour faire de l'achimie

  Peche:
	* Le personnage a la capacité de pecher des poissons.
	* Les poissons péchés varies en fonction de la localisation (lac, mer, ocean, etans)
	* La taille des poissons péchés est en fonction du niveau de peche du personnage et de la canne a peche utilisée avec une composante aléatoire
	* Le prix de vente des poissons est en fonction de la taille de ce dernier et de son espece.
	* Il est possible soit de vendre le poisson en entier soit du recuperer des elements du poisson (exemple= ecailles + viande de poisson) 

  Farming:
	* Le personnage a la capacité de Farmer des legumes et arbres fruitiers.

## Leveling

* Le personnage possède plusieurs leveling distincts:

  niveau du joueur:
	* Chaque niveau demande un montant d'XP croissant
	* XP gagnée après combat si combat gagné ou fuite après avoir tué un des monstres (gagne seulement l'XP du monstre tué)
	* Chaque niveau augmente les stats principales du personnage (attaque, defense, attaque speciale, etc...)
	* Deblocage de competances et autres evenements en fonction du niveau

  forgeronerie:
	* Chaque niveau demande un montant d'XP croissant
	* chaque element forgé donne au personnage un certain montant d'XP 
	* Le niveau de forgeronerie permet de debloquer de nouvelles possibilités de forgage

  Alchimie:
	* Chaque niveau demande un montant d'XP croissant
	* chaque element crafté donne au personnage un certain montant d'XP 
	* Le niveau d'alchimie permet de debloquer de nouvelles possibilités d'alchimie

  Peche:
	* Chaque niveau demande un montant d'XP croissant
	* chaque element crafté donne au personnage un certain montant d'XP 

  Farming:
	* Chaque niveau demande un montant d'XP croissant
	* chaque element recolté donne au personnage un certain montant d'XP 

## Economie

* pieces bronze argent or

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
