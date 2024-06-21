#include "include/Map.hpp"
#include "include/Square.hpp"
#include "include/Node.hpp"
#include "include/Graph.hpp"
#include "include/const.h"
#include "include/GLHelpers.hpp"

#include <glad/glad.h>
#include <fstream>
#include <iostream>
#include <sil/sil.hpp>
#include <string>
#include <sstream>

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

void Map::switchTowerMode() {
    if (!towerMode && coins > 5) {
        towerMode = true;
    } else {
        towerMode = false;
    }
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

            int uiX = i * SCALE + LEFT_MARGIN;
            int uiY = j * SCALE + TOP_MARGIN;

            // Check if the color is the same as the inColor
            if (colorMatch(color, inColor)) {
                row.push_back(Square(Square::IN, uiX, uiY));
            } else if (colorMatch(color, outColor)) {
                row.push_back(Square(Square::OUT, uiX, uiY));
            } else if (colorMatch(color, pathColor)) {
                row.push_back(Square(Square::PATH, uiX, uiY));
            } else {
                row.push_back(Square(Square::EMPTY, uiX, uiY));
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

void Map::loadTextures() {

    std::cout << "Loading textures" << std::endl;

    img::Image grassImg {img::load(make_absolute_path("images/grass3.png", true), 3, false)};
    grassTexture = loadTexture(grassImg);

    img::Image pathImg {img::load(make_absolute_path("images/path.png", true), 3, false)};
    pathTexture = loadTexture(pathImg);

    img::Image inImg {img::load(make_absolute_path("images/in.png", true), 3, false)};
    inTexture = loadTexture(inImg);

    img::Image outImg {img::load(make_absolute_path("images/out.png", true), 3, false)};
    outTexture = loadTexture(outImg);

    img::Image hammerImg {img::load(make_absolute_path("images/hammer.png", true), 3, false)};
    hammerTexture = loadTexture(hammerImg);

}

void Map::setup(std::string pngPath, std::string itdPath) {
    this->loadMapFromFile(pngPath, itdPath);
    this->loadGraphFromFile(itdPath);
    this->loadTextures();
}

void Map::render() {
    /*
     * Utiliser LEFT_MARGIN et TOP_MARGIN pour calculer les marges à gauche et en haut
     */

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

    // Utiliser les marges pour aligner à gauche et en haut
    float offsetX = LEFT_MARGIN;
    float offsetY = HEIGHT - (height * SCALE) - TOP_MARGIN;

    // Dessiner chaque carré
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            Square& square = squares[x][y]; // Accéder aux cases par colonne puis par ligne

            // Calculer les coordonnées avec les marges
            float xPos = offsetX + x * SCALE;
            float yPos = offsetY + (height - y - 1) * SCALE; // Inverser y pour correspondre à l'axe vertical

            if (square.type == Square::EMPTY) {
                drawTexturedQuad(xPos, yPos, SCALE, SCALE, grassTexture, false, square.isHovered);
            } else if (square.type == Square::PATH) {
                drawTexturedQuad(xPos, yPos, SCALE, SCALE, pathTexture, false, square.isHovered);
            } else if (square.type == Square::IN) {
                drawTexturedQuad(xPos, yPos, SCALE, SCALE, inTexture, false, square.isHovered);
            } else if (square.type == Square::OUT) {
                drawTexturedQuad(xPos, yPos, SCALE, SCALE, outTexture, true, square.isHovered);
            } else {
                glm::vec3 color = getColor(square.type);

                // Dessiner le carré coloré
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


    /*
     * Render des tours
     */

    if (towers.size() > 0) {
        for (auto& tower : towers) {
            tower.render();
        }
    }

    if (enemies.size() > 0) {
        for (auto& enemy : enemies) {
            enemy.render();
        }
    }

    if (lives <= 0) {
        gameOver = true;
    }

    // Indicatif build mode
    if (towerMode) {
        drawTexturedQuad(WIDTH - 100, HEIGHT - 100, SCALE, SCALE, hammerTexture, true, false);
    }

}

void Map::createEnemy() {
    Enemy enemy(2, 100, graph.findNodeById(0), graph, &lives);

    Node* inNode = graph.findNodeById(0);
    Node* outNode = graph.getLastNode();

    std::vector<int> path = graph.getPath(*inNode, *outNode);

    enemy.path = path;

    enemy.nextNode = graph.findNodeById(path[0]);

    enemies.push_back(enemy);

}

void Map::updateEnemies() {
    for (auto& enemy : enemies) {
        enemy.update(elapsedTime);
    }
}



void Map::updateHoverSquare() {
    if (towerMode) {
        // On itère sur les squares
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                Square& square = squares[i][j];
                if (xCursor >= square.uiX && xCursor <= square.uiX + SCALE && yCursor >= square.uiY && yCursor <= square.uiY + SCALE) {
                    square.isHovered = true;
                } else {
                    square.isHovered = false;
                }
            }
        }

    }
}

void Map::placeTower() {
    if (towerMode && coins >= 5) {

        // On itère sur les squares
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                Square& square = squares[i][j];
                if (square.isHovered && square.type == Square::EMPTY) {

                    Tower tower(&square);
                    towers.push_back(tower);
                    coins -= 5;

                    std::cout << "Tower placed" << std::endl;
                }
            }
        }
    }
}

void Map::update() {
    // Pour mettre tout ce qui doit etre check à chaque frame mais qui n'est pas du render

    enemyTimer += elapsedTime;

    if (enemyTimer > 5.0) {
        createEnemy();
        enemyTimer = 0;
    }

    updateEnemies();
}