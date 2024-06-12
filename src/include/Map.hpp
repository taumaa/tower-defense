#pragma once

#include "Square.hpp"
#include "Graph.hpp"
#include <vector>
#include <string>
#include <map>
#include <sil/sil.hpp>

class Map {

    public:

        static const int SCALE = 60; // Une case fait 50 px de côté

        int width;
        int height;

        glm::vec3 inColor;
        glm::vec3 outColor;
        glm::vec3 pathColor;

        // Matrix of squares using vector of vectors
        std::vector<std::vector<Square>> squares;
        Graph graph;

        bool validateItd(std::string itdPath);
        void loadMapFromFile(std::string pngPath, std::string itdPath);
        void loadGraphFromFile(std::string itdPath);
        void setup(std::string pngPath, std::string itdPath);

        // To delete
        void test();

        void render();
};