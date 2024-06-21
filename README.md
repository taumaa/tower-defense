# Projet CPP - Tower Defense

## Présentation

Notre projet est un jeu de Tower Defense réalisé en C++ avec OpenGL. Le but du jeu est de placer des tours sur une carte pour empêcher des ennemis d'atteindre une base qu'il faut défendre. Les ennemis suivent un chemin prédéfini et les tours peuvent être placées sur des cases vides. Les tours tirent automatiquement sur les ennemis qui passent à leur portée.
Les assets graphiques sont dans un style pixel art et le jeu est entièrement en 2D

![Texte alternatif](/screenshots/menu.png)

Le fonctionnement du jeu est le suivant :
- Une fois sur le menu le joueur devra appuyer sur "E" pour lancer la partie
- Ensuite, il pourra commencer à placer des tours sur la carte. Pour ca il devra rentrer en "construction" en appuyant sur "T". Evidemment, le joueur ne peut placer des tours que sur des cases à l'exterieur du chemin. Chaque tour coute 5 pièce à construire.
- Les ennemis vont apparaitre à intervalle régulier et vont suivre le chemin prédéfini.
- Si ils atteignent la base au bout du chemin, ils commenceront à l'attaquer et à lui faire perdre des points de vie. Si les points de vie atteignent 0, c'est la fin de la partie le joueur a perdu. 

![Texte alternatif](/screenshots/towers.png)

- Quand le joueur n'a plus de points de vie, un écran "Game Over" apparait.

![Texte alternatif](/screenshots/over.png)

- A tout moment le joueur peut mettre en pause le jeu en appuyant sur "P".
- Pour quitter le jeu, il suffit d'appuyer sur "Q".


## Structure du code
Le code est divisé en de nombreuses classes, représentant chacune une partie de l'application. 
Le gros de l'application est réalisé dans la classe Map. C'est en quelque sorte la "classe mère", qui va contenir une grosse partie de la logique.
Elle va par exemple contenir la liste de nos tours, de nos ennemies, le chargement et le rendu de la map. 

```c++

class Map {

    public:

        static const int SCALE = 60; // Une case fait 60 px de côté

        int width;
        int height;

        int coins = 100;
        int lives = 100;

        int xCursor;
        int yCursor;

        double elapsedTime = 0.0;

        double enemyTimer;

        bool towerMode = false;

        glm::vec3 inColor;
        glm::vec3 outColor;
        glm::vec3 pathColor;

        // Matrix of squares using vector of vectors
        std::vector<std::vector<Square>> squares;
        std::vector<Tower> towers;
        std::vector<Enemy> enemies;
        Graph graph;

        /*
         * Texture
         */
        GLuint grassTexture {};
        GLuint pathTexture {};
        GLuint inTexture {};
        GLuint outTexture {};
        GLuint hammerTexture {};



        bool validateItd(std::string itdPath);
        void loadMapFromFile(std::string pngPath, std::string itdPath);
        void loadGraphFromFile(std::string itdPath);
        void loadTextures();

        void setup(std::string pngPath, std::string itdPath);

        void switchTowerMode();
        void placeTower();

        void createEnemy();
        void updateEnemies();

        void render();
        void updateHoverSquare();
        void update();


};
```

Initialement, nous voulions diviser le code en une classe "Game" parente et une classe "Map" qui gère uniquement le chargement et l'affichage de la page. Cependant ce choix s'est averé trop compliquer notamment lorsque des fonctions avaient besoin d'utiliser des attributs d'autres classes.


Ensuite, nous avons des classes pour chaque élément de notre jeu : Les tours, les ennemis, les cases de la map, noeuds, etc.
Ces classes sont relativement simples et contiennent qui gèrent la logique de l'élément à chaque frame ainsi que son rendu.
Par exemple pour les ennemis nous allons venir les bouger à chaque frame et les faire avancer sur le chemin prédéfini.

Le graphe représentant la map a aussi sa propre classe, ainsi que les noeuds qui le composent.

## Post mortem
Le jeu en l'état n'est pas totalement finit. Le principale fonctionnalité manquante est la gestion des projectiles des tours. En l'état les tours n'attaquent pas les ennemis ce qui est quand même assez problématique pour un jeu de tower defense.
Globalement, nous avons eu un peu de mal à nous organiser et à nous répartir les tâches. Nous avons donc perdu un peu de temps à essayer de se mettre d'accord sur la structure du code et sur les différentes classes à créer.

Nous avons également sous-estimé le temps nécessaire pour réaliser le projet. Nous avons eu du mal à gérer le temps ce qui nous à mit très en retard vis à vis de la deadline.

Un autre élément mal réalisé est la gestion des graphes. Dans l'état actuel, notre map est bel et bien représenté par un graphe, mais les ennemis suivent le graphe linéairement. C'est à dire qu'il vont au noeud 1, puis au noeud 2, etc... Nous avons pas réussit à implémenter l'algorithme de Dijkstra pour calculer le chemin entre 2 noeud. 

La gestion des erreurs aurait également pu être amélioré. Par contre notre application ne vérifie pas la conformité du fichier ITD. Si un fichier ne respectant pas le bon format est fournit l'application risque de planter.

Enfin nous avons surtout rencontré des problèmes avec OpenGL, notamment sur la gestion des images et des textures.
Par exemple, nous avons pas réussit à intégrer des PNG transparent dans l'application. A chaque fois, OpenGL ajoutait un fond noir à l'image. Ainsi, nous avons été obligé de ne mettre que des images avec un fond dans notre application
