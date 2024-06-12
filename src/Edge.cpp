#include "include/Edge.hpp"

// Constructeur pour initialiser une arête avec une source, une destination et un poids
Edge::Edge(Node* source, Node* destination, double weight) : source(source), destination(destination), weight(weight) {}

Node* Edge::getSource() const {
    return source;
}

Node* Edge::getDestination() const {
    return destination;
}

double Edge::getWeight() const {
    return weight;
}
