#pragma once

#include "Map.hpp"

class Game {
    public:
        void unpause();
        void pause();
        bool isInMenu = true;
        bool isPaused = false;

        Map map;

};