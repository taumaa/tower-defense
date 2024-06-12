#include "include/Map.hpp"
#include "include/Square.hpp"
#include "include/Graph.hpp"

#include <glad/glad.h>
#include <fstream>
#include <iostream>
#include <sil/sil.hpp>
#include <string>
#include <sstream>


bool Map::validateItd(std::string itdPath) {
    // TODO: A implementer
    return true;
}

bool colorMatch(glm::vec3 color1, glm::vec3 color2) {
    /*
     * Check pour chaque composante de la couleur (r, g, b) si ca match avec la deuxième
     * Prend 1% de tolérance psk sinon ca bug (il trouve des 0.001 au lieu de 0)
     */

    const float tolerance = 0.05f;

    // TODO: On doit pouvoir enlever la tolérance car png fixed
    float diffR = std::abs(color1.r - color2.r);
    float diffG = std::abs(color1.g - color2.g);
    float diffB = std::abs(color1.b - color2.b);


    bool res = (diffR <= tolerance && diffG <= tolerance && diffB <= tolerance);

    return res;
}

void Map::loadMapFromFile(std::string pngPath, std::string itdPath) {

    /*
     * Idee: Lire le itd pour obtenir les couleurs, les sauvegarder
     * puis lire le png pour obtenir les couleurs et les comparer
     *
     */

    std::cout << "Loading map from " << pngPath << " and " << itdPath << std::endl;

    // Ouvrir le fichier itd et check s'il existe bien
    std::ifstream itdFile(itdPath);
    if (!itdFile.is_open()) {
        std::cerr << "Unable to open itd file: " << itdPath << std::endl;
        return;
    }

    std::string line;
    // Lire le fichier itd et sauvegarder les couleurs dans la classe
    while (std::getline(itdFile, line)) {
        std::istringstream iss(line);
        std::string type;
        int r, g, b;
        if (iss >> type >> r >> g >> b) {
            glm::vec3 color(r/255.0f, g/255.0f, b/255.0f);
            if (type == "in") {
                inColor = color;
            } else if (type == "out") {
                outColor = color;
            } else if (type == "path") {
                pathColor = color;
            }
        }
    }

    // Ouvrir l'image
    sil::Image imgFile{pngPath};

    // Hauteur et largeur de la map
    width = imgFile.width();
    height = imgFile.height();

    std::cout << "Width: " << width << " Height: " << height << std::endl;


    // Iterer sur l'image pour obtenir notre matrice avec les bonnes cases
    for (int i = 0; i < width; i++) {
        std::vector<Square> row;
        for (int j = 0; j < height; j++) {

            int inverseHeight = height - j - 1 ;

            // Get the color of the pixel
            glm::vec3 color = imgFile.pixel(i, inverseHeight);

            // Check if the color is the same as the inColor
            if (colorMatch(color, inColor)) {
                row.push_back(Square(Square::IN));
            } else if (colorMatch(color, outColor)) {
                row.push_back(Square(Square::OUT));
            } else if (colorMatch(color, pathColor)) {
                row.push_back(Square(Square::PATH));
            } else {
                row.push_back(Square(Square::EMPTY));
            }
        }
        squares.push_back(row);
    }

}

void Map::loadGraphFromFile(std::string itdPath) {
    std::cout << "Loading graph from " << itdPath << std::endl;

    std::ifstream file(itdPath);
    std::string line;
    Graph newGraph;

    std::unordered_map<int, std::tuple<int, int, int>> nodes; // Map pour stocker les informations des nœuds

    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string token;
            iss >> token;

            if (token == "graph") {
                int numberOfNodes;
                iss >> numberOfNodes;
                // Le nombre de nœuds peut être utilisé si nécessaire
            }
            else if (token == "node") {
                int id, x, y, connectedNodeId;
                iss >> id >> x >> y;

                if (iss >> connectedNodeId) {
                    nodes[id] = std::make_tuple(x, y, connectedNodeId);
                } else {
                    nodes[id] = std::make_tuple(x, y, -1); // -1 signifie pas de nœud connecté
                }
            }
        }
        file.close();
    } else {
        std::cerr << "Unable to open file";
        return;
    }

    // Ajout des nœuds au graphe
    for (const auto& node : nodes) {
        int id = node.first;
        int x = std::get<0>(node.second);
        int y = std::get<1>(node.second);
        newGraph.addNode(id, x, y);
    }

    // Ajout des arêtes au graphe
    for (const auto& node : nodes) {
        int id = node.first;
        int connectedNodeId = std::get<2>(node.second);
        if (connectedNodeId != -1) {
            newGraph.addEdge(id, connectedNodeId, 1.0); // Poids par défaut de 1.0, ajustez si nécessaire
        }
    }

    graph = newGraph;


}

void Map::render() {

    // Couleurs en RGB
    auto getColor = [this](Square::SquareType type) -> glm::vec3 {
        switch (type) {
            case Square::PATH:
                return pathColor;
            case Square::IN:
                return inColor;
            case Square::OUT:
                return outColor;
            case Square::EMPTY:
            default:
                return glm::vec3(0, 0, 0); // Noir pour les cases vides
        }
    };

    // Taille de la fenêtre en dur
    int windowWidth = 1280;
    int windowHeight = 720;

    // Calculer les décalages pour centrer la grille
    float offsetX = (windowWidth - (width * SCALE)) / 2.0f;
    float offsetY = (windowHeight - (height * SCALE)) / 2.0f;

    // Dessiner chaque carré
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            Square& square = squares[x][y]; // Accéder aux cases par colonne puis par ligne

            glm::vec3 color = getColor(square.type);

            // Calculer les coordonnées avec le décalage centré
            float xPos = offsetX + x * SCALE;
            float yPos = offsetY + (height - y - 1) * SCALE; // Inverser y pour correspondre à l'axe vertical

            // Dessiner le carré
            glColor3f(color.r, color.g, color.b); // Normalisez les couleurs
            glBegin(GL_QUADS);
                glVertex2f(xPos, yPos);
                glVertex2f(xPos + SCALE, yPos);
                glVertex2f(xPos + SCALE, yPos + SCALE);
                glVertex2f(xPos, yPos + SCALE);
            glEnd();
        }
    }
}

void Map::setup(std::string pngPath, std::string itdPath) {
    this->loadMapFromFile(pngPath, itdPath);
    this->loadGraphFromFile(itdPath);
}

void Map::test() {
    std::cout << "TEST IS ACTIVATED" << std::endl; // ne pas delete

    std::cout << "X and Y of 2: " << graph.nodes[2].x << " " << graph.nodes[2].y << std::endl;
    std::cout << "X and Y of 3: " << graph.nodes[3].x << " " << graph.nodes[3].y << std::endl;
    std::cout << "X and Y of 4: " << graph.nodes[4].x << " " << graph.nodes[4].y << std::endl;
    std::cout << "X and Y of 5: " << graph.nodes[5].x << " " << graph.nodes[5].y << std::endl;

}