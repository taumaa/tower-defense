#pragma once
#include "Enemy.hpp"
#include <vector>


class Wave {
public:

    int difficulty;

    vector<Enemy> enemies;

    void setupEnemies();
};