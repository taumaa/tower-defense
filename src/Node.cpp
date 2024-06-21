#include "include/Node.hpp"
#include "include/const.h"

#include <iostream>
#include <glad/glad.h>
#include <string>
#include "simpletext.h"



// Constructeur pour initialiser un nœud avec un identifiant et des coordonnées
Node::Node(int id, int x, int y){
    this->id = id;
    this->x = x;
    this->y = y;

    // Calculer les coordonnées de l'interface utilisateur
    uiX = (x * SCALE - SCALE / 2) + LEFT_MARGIN + SCALE;
    uiY = HEIGHT - ((y * SCALE - SCALE / 2) + TOP_MARGIN + SCALE);

}

int Node::getId() const {
    return id;
}

void Node::render() {
    SimpleText TextRenderer {};
    TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::CYAN);
    TextRenderer.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 0.f, 0.f);

    // Dessiner l'id du noeud sur ses coordonnées uiX et uiY
    // Utiliser uiX et uiY pour les coordonnées

    int nodeId = id;

    TextRenderer.Label(std::to_string(nodeId).c_str(), uiX, HEIGHT -    uiY, SimpleText::CENTER);

    TextRenderer.Render();
}

