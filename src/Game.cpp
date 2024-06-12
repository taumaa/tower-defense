#include "include/Game.hpp"
#include <glad/glad.h>

// Définition de la méthode unpause
void Game::unpause() {
    if (isPaused) {
        isPaused = false;
        // Ajoutez ici les actions à effectuer lors de la reprise du jeu
    }
}

// Définition de la méthode pause
void Game::pause() {
    if (!isPaused) {
        isPaused = true;
    }
}