#pragma once
#include "Node.hpp"
#include "Graph.hpp"
#include <vector>
#include <sil/sil.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>


class Enemy {
    public:
        int uiX;
        int uiY;

        int speed;
        int hp;
        int* userHp;

        Node* currentNode;
        Node* nextNode;
        Node* destinationNode;

        GLuint enemyTexture {};

        std::vector<int> path;

        double timer;


        Graph graph;

        Enemy(int speed, int hp, Node* startNode, Graph graph, int* userHp);
        Enemy(); // Constructeur par défaut



        void setup();
        void render();

        void update(double elapsedTime);
        void isOnCurrentNode();
        void isOnNextNode();
        void isOnDestinationNode();
        void isOnPath();
};