#include "include/Node.hpp"

// Constructeur pour initialiser un nœud avec un identifiant et des coordonnées
Node::Node(int id, int x, int y) : id(id), x(x), y(y) {}

int Node::getId() const {
    return id;
}
