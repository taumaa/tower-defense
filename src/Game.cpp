#include "include/Game.hpp"
#include <glad/glad.h>
#include "simpletext.h"
#include "include/const.h"
#include <string>


void Game::switchPause() {
    isPaused = !isPaused;
}

void Game::setup() {
    TextRenderer.ResetFont();
    TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::WHITE);
    TextRenderer.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 0.f, 0.f);
    TextRenderer.EnableBlending(true);

    map.setup("maps/map.png", "maps/map.itd");
}

void Game::render() {
    map.render();

    // pour être aligné
    int textX = WIDTH / 2 + 50;
    int textBaseY = HEIGHT / 2 - 200 ;


    // nb de pièces
    std::string coinsText = "Pieces: " + std::to_string(map.coins);
    const char* coinsStr = coinsText.c_str();
    TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::BLACK);
    TextRenderer.SetTextSize(SimpleText::SIZE_32);
    TextRenderer.Label(coinsStr, textX, textBaseY, SimpleText::LEFT);
    TextRenderer.Render();

    // nb de vies
    std::string livesText = "Points de vies: " + std::to_string(map.lives);
    const char* livesStr = livesText.c_str();
    TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::BLACK);
    TextRenderer.SetTextSize(SimpleText::SIZE_32);
    TextRenderer.Label(livesStr, textX, textBaseY + 50, SimpleText::LEFT);
    TextRenderer.Render();

    // texte pause
    if (!isPaused) {
        TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::BLACK);
        TextRenderer.SetTextSize(SimpleText::SIZE_16);
        TextRenderer.Label("Appuyez sur P pour mettre en pause", textX, HEIGHT - 200, SimpleText::LEFT);
        TextRenderer.Render();
    }

    TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::BLACK);
    TextRenderer.SetTextSize(SimpleText::SIZE_16);
    TextRenderer.Label("Appuyez sur T pour construire une tour", textX, HEIGHT - 160, SimpleText::LEFT);
    TextRenderer.Render();

    TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::BLACK);
    TextRenderer.SetTextSize(SimpleText::SIZE_16);
    TextRenderer.Label("Appuyez sur Q pour quitter", textX, HEIGHT - 120, SimpleText::LEFT);
    TextRenderer.Render();
}