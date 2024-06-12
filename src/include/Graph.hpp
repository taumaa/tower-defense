#pragma once
#include <vector>
#include "Node.hpp"
#include "Edge.hpp"
#include <unordered_map>
#include <queue>

// Classe représentant un graphe composé de sommets et d'arêtes
class Graph {
public:
    Graph();
    void addNode(int id, double x, double y);
    void addEdge(int sourceId, int destinationId, double weight);
    std::vector<int> dijkstra(const Node& source, const Node& destination);

    std::vector<Node> nodes;
    std::vector<Edge> edges;

private:

    Node* findNodeById(int id);
    std::unordered_map<int, std::vector<Edge>> adjacencyList;
    void buildAdjacencyList();
};
