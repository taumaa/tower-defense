#pragma once
#include "Node.hpp"

// Classe représentant une arête dans le graphe
class Edge {
public:
    Edge(Node* source, Node* destination, double weight);
    Node* getSource() const;
    Node* getDestination() const;
    double getWeight() const;

private:
    Node* source;
    Node* destination;
    double weight;  // Distance ou coût de déplacement entre les nœuds
};
