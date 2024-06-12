#include "include/Graph.hpp"
#include <limits>
#include <unordered_map>
#include <set>
#include <algorithm>

// Constructeur pour initialiser un graphe vide
Graph::Graph() {}

// Ajoute un nœud au graphe
void Graph::addNode(int id, double x, double y) {
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

// Construit la liste d'adjacence à partir des arêtes
void Graph::buildAdjacencyList() {
    adjacencyList.clear();
    for (const auto& edge : edges) {
        adjacencyList[edge.getSource()->getId()].push_back(edge);
    }
}

// Algorithme de Dijkstra pour trouver le plus court chemin entre deux nœuds
std::vector<int> Graph::dijkstra(const Node& source, const Node& destination) {
    std::unordered_map<int, double> distances;
    std::unordered_map<int, int> previous;
    std::set<std::pair<double, int>> nodesToVisit;

    for (const auto& node : nodes) {
        distances[node.getId()] = std::numeric_limits<double>::infinity();
        previous[node.getId()] = -1;
    }

    distances[source.getId()] = 0.0;
    nodesToVisit.insert({0.0, source.getId()});

    while (!nodesToVisit.empty()) {
        int currentNodeId = nodesToVisit.begin()->second;
        nodesToVisit.erase(nodesToVisit.begin());

        if (currentNodeId == destination.getId()) {
            std::vector<int> path;
            for (int at = destination.getId(); at != -1; at = previous[at]) {
                path.push_back(at);
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (const auto& edge : adjacencyList[currentNodeId]) {
            int neighborId = edge.getDestination()->getId();
            double newDist = distances[currentNodeId] + edge.getWeight();

            if (newDist < distances[neighborId]) {
                nodesToVisit.erase({distances[neighborId], neighborId});
                distances[neighborId] = newDist;
                previous[neighborId] = currentNodeId;
                nodesToVisit.insert({newDist, neighborId});
            }
        }
    }

    return {}; // Retourne un chemin vide si aucun chemin n'est trouvé
}
