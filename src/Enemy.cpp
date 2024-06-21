#include "include/Enemy.hpp"
#include <iostream>
#include <string>
#include <glad/glad.h>
#include "include/const.h"
#include "include/Node.hpp"
#include "include/GLHelpers.hpp"
#include "include/utils.hpp"


void Enemy::setup() {
    /*
     * - Assigner les coordonnées x et y du début de son chemin
     * - Calculer son noeud de destination
     * - Le mettre en mouvement
     */

    std::cout << "Enemy setup" << std::endl;
    if (currentNode != nullptr) {
        uiX = currentNode->uiX;
        uiY = currentNode->uiY;

        int nextNodeId = currentNode->getId() + 1;
        nextNode = graph.findNodeById(nextNodeId);

        destinationNode = graph.getLastNode();

    }

    img::Image enemyImg {img::load(make_absolute_path("images/creeper.png", true), 3, false)};
    enemyTexture = loadTexture(enemyImg);


}

Enemy::Enemy(int speed, int hp, Node* startNode, Graph graph, int* userHp) {
    this->speed = speed;
    this->hp = hp;
    this->currentNode = startNode;
    this->destinationNode = nullptr;
    this->graph = graph;
    this->userHp = userHp;
    setup();
}

Enemy::Enemy() {
    this->speed = 0;
    this->hp = 0;
    this->currentNode = nullptr;
    this->destinationNode = nullptr;
    setup();
}
void Enemy::render() {
    /*
     * - Dessiner l'ennemi à ses coordonnées x et y
     */

    // Paramètres du carré
    float width = 30.0f;
    float height = 30.0f;
    float centerX = static_cast<float>(uiX);
    float centerY = static_cast<float>(uiY);

    // Dessiner le carré avec la texture enemyTexture
    drawTexturedQuad(centerX - width / 2.0f, centerY - height / 2.0f, width, height, enemyTexture, true);
}


void Enemy::update(double elapsedTime) {

    timer += elapsedTime;

    if (uiX == destinationNode->uiX && uiY == destinationNode->uiY) {
        // Rien
        isOnDestinationNode();
    } else if (uiX == currentNode->uiX && uiY == currentNode->uiY) {
        // Deplacement vers nextNode
        isOnCurrentNode();


        // il est sur next node
    } else if (uiX == nextNode->uiX && uiY == nextNode->uiY) {
        // Deplacement vers destination node
        isOnNextNode();

        // Il est sur destination node
    } else {
        // Il est sur le chemin
        isOnPath();
    }
}


void Enemy::isOnCurrentNode() {
    /*
     * - Choisir le prochain noeud
     * - Se déplacer vers le prochain noeud
     */

    if (nextNode != nullptr) {

        if (nextNode->uiX < currentNode->uiX) {
            uiX -= speed;
        } else if (nextNode->uiX > currentNode->uiX) {
            uiX += speed;
        } else if (nextNode->uiY < currentNode->uiY) {
            uiY -= speed;
        } else if (nextNode->uiY > currentNode->uiY) {
            uiY += speed;
        }
    }
}

void Enemy::isOnNextNode() {
    /*
     * - Next node deviens le current node
     * - Nouveau next node en fonction du path
     */

    currentNode = nextNode;

    int nextNodeId = currentNode->getId() + 1;
    Node* next = graph.findNodeById(nextNodeId);
    nextNode = next;

}

void Enemy::isOnDestinationNode() {
    /*
     * - Supprimer next node
     * - Rien d'autre
     */

    currentNode = destinationNode;

    // AttackMode
    if (timer > 1.0) {
        *userHp -= 1;
        timer = 0;
    }

}

void Enemy::isOnPath() {
    /*
     * - Si l'ennemi est sur le chemin on le deplace vers le prochain noeud
     */

    if (nextNode != nullptr) {

        if (nextNode->uiX < currentNode->uiX) {
            uiX -= speed;
        } else if (nextNode->uiX > currentNode->uiX) {
            uiX += speed;
        } else if (nextNode->uiY < currentNode->uiY) {
            uiY -= speed;
        } else if (nextNode->uiY > currentNode->uiY) {
            uiY += speed;
        }
    }
}



