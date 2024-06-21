#pragma once


#include "Map.hpp"
#include "simpletext.h"

class Game {
    public:
        void switchPause();
        bool isInMenu = true;
        bool isPaused = false;


        Map map;

        SimpleText TextRenderer {};

        void render();
        void setup();

};