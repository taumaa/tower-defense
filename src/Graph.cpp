#include "include/Graph.hpp"
#include <limits>
#include <unordered_map>
#include <set>
#include <iostream>
#include <algorithm>

// Constructeur pour initialiser un graphe vide
Graph::Graph() {}

// Ajoute un nœud au graphe
void Graph::addNode(int id, int x, int y) {
    nodes.emplace_back(id, x, y);
}

// Ajoute une arête au graphe avec un poids de passage
void Graph::addEdge(int sourceId, int destinationId, double weight) {
    Node* source = findNodeById(sourceId);
    Node* destination = findNodeById(destinationId);

    if (source && destination) {
        edges.emplace_back(source, destination, weight);
        buildAdjacencyList();
    }
}

// Trouve un nœud par son identifiant
Node* Graph::findNodeById(int id) {
    for (auto& node : nodes) {
        if (node.getId() == id) {
            return &node;
        }
    }
    return nullptr;
}

// Renvoie le dernier nœud du graphe
Node* Graph::getLastNode() {
    for (auto& node : nodes) {
        if (node.getId() == nodes.size() - 1) {
            return &node;
        }
    }
}

// Construit la liste d'adjacence à partir des arêtes
void Graph::buildAdjacencyList() {
    adjacencyList.clear();
    for (const auto& edge : edges) {
        adjacencyList[edge.getSource()->getId()].push_back(edge);
    }
}

std::vector<int> Graph::getPath(const Node& source, const Node& destination) {


    std::vector<int> path;
    for (int i = source.getId() + 1; i <= destination.getId(); i++) {
        path.push_back(i);
    }

    return path;
}