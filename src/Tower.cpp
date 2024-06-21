#include "include/Tower.hpp"
#include "include/const.h"
#include "include/utils.hpp"


void Tower::render() {
    // On récupère la position du carré sur lequel la tour est placée, ca correspond à en haut à gauche
    float x = positionSquare->uiX;
    float y = HEIGHT - positionSquare->uiY - SCALE;

    drawTexturedQuad(x, y, SCALE, SCALE, towerTexture, true, false);
}