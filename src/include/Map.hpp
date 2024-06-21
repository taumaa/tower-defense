#pragma once

#include "Square.hpp"
#include "Graph.hpp"
#include "Enemy.hpp"
#include "Tower.hpp"

#include <vector>
#include <string>
#include <map>
#include <sil/sil.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>
#include "include/utils.hpp"


class Map {

    public:

        static const int SCALE = 60; // Une case fait 60 px de côté

        int width;
        int height;

        int coins = 100;
        int lives = 100;

        bool gameOver = false;

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