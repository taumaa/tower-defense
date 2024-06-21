#include "include/Square.hpp"

void Square::render() {
    switch (type) {
        case PATH:
            // Dessiner un carré de couleur pathColor
            break;
        case IN:
            // Dessiner un carré de couleur inColor
            break;
        case OUT:
            // Dessiner un carré de couleur outColor
            break;
        case EMPTY:
            // Ne rien dessiner
            break;
    }
}