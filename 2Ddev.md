# Notre xp en dev de jeu 2D

***Introduction***

A travers cet article nous allons tenter de partager avec vous notre expérience dans le développement de jeu rétro 2D, principalement acquise pendant le développement d'un prototype : [isoLand](https://gitlab.com/ProjetsHUB/Dommerc)
Ici nous ne prétendons pas donner un enseignement complet. Nos compétences et connaissances dans ce domaine sont insuffisantes. Et pour être franc, nous ne faisons que de débuter dans ce domaine. Cependant nous avons quand même des choses à raconter que nous pensons intéressantes :)

> *Donc gardez à l'esprit que ce qui va suivre ne constitue en rien une
> vérité absolue, il s'agit simplement de notre expérience personnelle.
> Libre à vous de critiquer !*

***Pourquoi pas Unity ou Unreal et de la 3D ?***

Comme expliqué plus haut, nous sommes débutants. Du coup nous recherchons avant tout à apprendre des choses. La 3D complexifie directement la programmation. A un niveau modeste comme le notre et à notre échelle, réaliser quelque chose de concret en jeu 3D, induit forcément l'utilisation d'un engine haut niveau tel que *Unity* ou *Unreal*. Par conséquent on se retrouve sur un logiciel avec une interface utilisateur complète, à créer des diagrammes, importer des *assets* et taper quelques ligne de code. Okay, c'est hyper productif, hyper user-friendly et moderne mais on ne sait pas vraiment comment ça marche sous le capot. C'est principalement cet aspect que nous avons cherché à éviter en nous tournant vers la 2D. Histoire d'approcher un concept plus simple, pour apprendre plus simplement.
Le choix de l'engine c'est fait assez logiquement après quelques recherches et grâce au peu d'expérience que nous avions. Car oui, avant cocos2d, nous nous sommes pas mal amusé avec la *SFML* qui pour le coup est trop bas niveau pour un bon rapport productivité/temps de dev.
Nous voulions faire de la programmation de jeu vidéo 2D, certes sans trop de prémâché mais sans à avoir à implémenter soi-même les patterns designs qui forment le socle de tout engine qui se respecte (le Scene-graph, workflow des assets etc...). Et c'est là qu'entre en scène Cocos2d-x.

## Cocos2d-x

![cocos2d-x logo](http://www.cocos2d-x.org/images/logo.png)

[Cocos2d-x](http://www.cocos2d-x.org/products) est un framework open-source écrit en C++ principalement axé sur le développement de jeu vidéo 2D cross platform (Desktop et mobile).
Du coup avec Cocos2d-x, pas de GUI, on code vraiment !
C'est un framework de référence (maintenant ac *Unity*) pour le développement du jeu pour mobile.

Concrètement il s'agit d'une librairie C++ mettant à disposition du développer une API documentée. Elle permet notamment de gérer :

 - des scènes
 - des nodes
 - des sprites
 - des events
 - l'audio
 - des composants UI
 - de la physique (physics engine)
 - le rendu
 - ...
 
 et même de la 3D même si c'est très limité (le framework étant principalement axé sur la 2D).

## Le concept, isoLand

le projet : [isoLand](https://gitlab.com/ProjetsHUB/Dommerc)

"isoLand" se décompose en 2 mots:
 - **iso** pour perspective isométrique
 - **land** pour terre, monde, *tile map*

Le concept est le suivant : réaliser un prototype de jeu vidéo 2D en vue isométrique et monde ouvert.

***Vue isométrique***

2D n'est pas que synonyme de vue de côté ou vue de dessus. On peut simuler de la 3D et donc de la profondeur en 2D grâce à la technique de la perspective isométrique.

*side view*
![side view](https://ic.pics.livejournal.com/kolyablog/16060954/117173/117173_original.jpg)

top down view
![top down view](https://gematsu.com/wp-content/uploads/2017/03/RPG-Maker-Fes_03-29-17.jpg)

*isometric view*
![isometric view](https://www.upsidelearning.com/blog/wp-content/uploads/2010/03/illusions-placement-objects-scenario-environment-e1269942786470.jpg)

Cette technique permet de rendre des graphiques 2D plus "réalistes" et ainsi de proposer une expérience de jeu plus immersive.

*exemple*
![exemple](https://steamcdn-a.akamaihd.net/steam/apps/108600/ss_dde6e8f01a85b4f6103ee9c006e8716fcd2f78ec.1920x1080.jpg?t=1508165065)

***Texture - Sprite quésaco ?***

La plupart d'entre vous sont déjà familiers avec ces deux entités très communes, allons donc à l'essentiel.
Une texture est une image. Mais elle s'appelle "texture" car elle a un rôle bien précis : être "plaquée" sur une entité 2D.
Un sprite quant à lui n'est rien de plus qu'un rectangle texturé.

![Sprite](https://www.sfml-dev.org/tutorials/2.0/images/graphics-sprites-definition.png)

Le sprite possède une notion d'entité : Le fond de l'écran constitue généralement le décor et les sprites sont les personnages et les objets qui se superposent au fond d'écran et qui peuvent se déplacer, être animé.

***Tile map***

![Tile map](https://orig00.deviantart.net/fbae/f/2015/032/9/f/desert_tilemap_by_neoz7-d8g8zxj.png)

En parlant 2D et monde ouvert, on parle aussi de *tile map*. Une tile map est une carte constituée d'un ensemble de tuiles de même dimension. Chaque tuile peut être texturée pour former le *level-design*.

![Tiled](http://www.mapeditor.org/img/tiled-logo-white.png)

Pour créer des tile map simplement il existe un logiciel open-source [Tiled](http://www.mapeditor.org/)
Il a une très bonne documentation et vous trouverez beaucoup de tutoriels pour prendre en main ce logiciel. De plus il est vraiment épuré et simple d'utilisation.
L'ingrédient essentiel d'une tile map c'est le *tile set*. Une image qui regroupe l'ensemble des différentes textures utilisées pour remplir chaque tuile de la map.
Avec Tiled on peut créer une carte, puis importer un tile set, la texturer, y ajouter différentes données puis l'exporter pour être utilisé dans un engine tel que cocos2d-x. Grossièrement ce que fait Tiled c'est créer un fichier au format xml qui permet d'énumérer pour chaque case de la carte, la texture et les données correspondantes qui y sont rattachées.

***Les Animations***

Cocos2d supporte deux types d'animation bien connus dans le développement de jeu 2D :

 - *Skeletal animation*
 - *Sprite sheet animation*

La première est une technique plus récente que l'autre. Avec la skeletal animation, l'animation est produite par différentes partis du corps articulées entre elles avec une relation parent-enfant. En gros c'est comme une marionnette.
Donc, pour créer une animation squelettique, vous devez créer un ensemble d'os connectés les uns aux autres via une relation parent / enfant de sorte que si vous animez l'os parent, tous ses os enfants sont animés. L'ensemble des os interconnectés représente le squelette. Après avoir créé l'ensemble d'os dont vous avez besoin pour votre animation, vous devez définir toutes les étapes clés pour toutes les transformations possibles. Finalement il ne vous reste plus qu'a lier un sprite à chaque os.
Exemple d'un sprite kit pour une skeletal animation :

![Skeletal sprite kit](https://image.noelshack.com/fichiers/2018/13/5/1522406095-skeleton-character-sheet.jpg)

La seconde, bien plus ancienne et toujours une des plus utilisée aujourd'hui pour son rendu fidèle au jeux 2D de l'époque, est la sprite sheet animation.
Elle repose sur un concept simple, pour animer un personnage, on fait défiler plusieurs images les unes après les autres donnant alors l'impression de mouvement.
Le problème pour l'économie de mémoire est qu'on a besoin de plusieurs images. Une image pour chaque étape de l'animation. A cela s'ajoute les informations inutiles de couleur sensées disparaître à chaque frame. Il s'agit des pixels transparents pour la plupart du temps qui entour le reste de l'image (mais ils peuvent être aussi d'une couleur particulière qui sera masqué à l'affichage).
On se retrouve alors avec un grande quantité d'information inutile que la RAM devra charger en permanence.
Et c'est là qu'interviennent les sprite *sheet*. L'idée réside dans le fait qu'il est plus facile pour le processeur graphique de gérer une seule image contenant toutes les informations dont il aura besoin pour animer. Du coup, plutôt que d'avoir à charger en mémoire (RAM) toutes les images de l'animation et de les afficher une par une. On les regroupe dans une seule et même image, qu'on charge une fois seulement en mémoire. Enfin pour chaque frame, on affiche la sélection de l'image correspondant au mouvement.

![Mario sprite sheet](http://saltares.com/blog/img/wp/mario-jump.png)

Bon maintenant une question se pose. 

> *Laquelle de ces deux techniques utiliser ?*

Vous vous doutez bien qu'une réponse toute faite n'existe pas pour répondre à cette problématique. Cela varie en fonction des besoins du projet, du rendu recherché etc...
Mais nous allons vous donner les principaux critères de chacune des techniques, ce qui vous permettra de vous faire une idée.

*Skeletal animation*

**avantages**
 Très performant en mémoire (RAM)
 Animation ultra fluide et naturelle
 Possibilité de créer de l'animation procédurale
 Possibilité de créer de l'animation en runtime
 Requiert moins de compétence en création graphique
 
**inconvénients**
Requiert beaucoup de travail de la part du processeur (CPU)
Création du squelette complexe
Ne correspond pas au rendu des jeux 2D rétro

----------

*Sprite sheet animation*

**avantages**
Charge rapidement en mémoire
Réduit le travail du processeur graphique (draw calls) et induit de meilleur performance
Est utilisée depuis plus longtemps que la skeletal animation -> beaucoup de tutoriels, de guide online
Même technique / même rendu que les jeux 2D d’antan et ça, ça n'a pas de prix !
 
**inconvénients**
Requiert beaucoup de travail artistique et graphique (une image pour chaque frame, pour chaque animation)
Si vous avez besoin de fluidité dans l'animation (ce qui est un peu contradictoire avec une expérience 2D rétro), la sprite sheet animation est en général moins fluide qu'une skeletal animation
Augmentation de l’utilisation de la mémoire pour un jeu qui requiert beaucoup d'animations différentes et de sprites différents

En ce qui concerne isoLand, nous avons naturellement opté pour les sprite sheets car nous étions à la recherche d'un rendu retro. Et nous sommes tombé sur un outil bien sympa pour les optimiser.

***Optimiser ses sprite cheets***

![enter image description here](https://cdn.codeandweb.com/o/img/texturepacker512-512.png)

L'outil [TexturePacker](https://www.codeandweb.com/texturepacker) permet d'optimiser ses textures. Tout en préservant leur qualité visuel, il va les compresser, permettant ainsi un gain de performance non négligeable sur la mémoire. Mais aussi grâce à des algorithmes puissants il peut réduire les espaces inutiles des sprites sheet induisant ainsi une économie de mémoire. Car même si un pixel d'une texture est transparent, à l'importation de celle-ci dans la mémoire, ce pixel existe il nécessite d'être traité. Que du bon pour l'optimisation du jeu final ;)

## Conclusion

Avec les différents outils et techniques qui viennent d'être énumérés, les possibilités de création de jeu 2D de qualité sont grandes ! Même à petite échelle ils vous permettrons de développer un projet abouti. Bien-sur une des clés de réussite dans le développement de jeu vidéo reste la collaboration de différents talents comme les programmeurs, les graphistes etc...

Voila, nous espérons vous avoir apporté 2-3 trucs !
