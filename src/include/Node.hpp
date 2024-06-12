#pragma once

// Classe représentant un nœud dans le graphe
class Node {
public:
    Node(int id, int x, int y);
    int x;
    int y;
    int id;


    int getId() const;
};