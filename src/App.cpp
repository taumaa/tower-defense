#include "include/App.hpp"
#include "include/const.h"
#include "include/utils.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>
#include "include/GLHelpers.hpp"

#include <sstream>
#include <iostream>

#include "simpletext.h"
#include "include/utils.hpp"
#include "include/GLHelpers.hpp"

App::App() : _previousTime(0.0), _viewSize(2.0) {
   // load what needs to be loaded here (for example textures)

    img::Image test {img::load(make_absolute_path("images/grass.png", true), 3, false)};
    
    _texture = loadTexture(test);
}

void App::setup() {
    // Set the clear color to a nice blue
    //glClearColor(0.5f, 0.7f, 1.0f, 1.0f);;

    // Setup the text renderer with blending enabled and white text color
    TextRenderer.ResetFont();
    TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::WHITE);
    TextRenderer.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 0.f, 0.f);
    TextRenderer.EnableBlending(true);

    game.setup();

    std::cout << "Setup called" << std::endl;

}


void App::renderBackground() {
    img::Image skyImg {img::load(make_absolute_path("images/sky.png", true), 3, false)};
    GLuint skyTexture = loadTexture(skyImg);
    drawTexturedQuad(0.0f, 0.0f, WIDTH, HEIGHT, skyTexture, true);
}


void App::renderMenu() {
    // Ecrit un texte noir centré à l'écran "Appuyez sur E pour commencer à jouer" en utilisant la police de taille 24
    TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::BLACK);
    TextRenderer.SetTextSize(SimpleText::SIZE_32);
    TextRenderer.Label("Appuyez sur E pour commencer a jouer", WIDTH / 2, HEIGHT / 2, SimpleText::CENTER);
    TextRenderer.Render();
}

void App::renderGameOver() {
    // Ecrit un texte noir centré à l'écran "Appuyez sur E pour commencer à jouer" en utilisant la police de taille 24
    TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::RED);
    TextRenderer.SetTextSize(SimpleText::SIZE_32);
    TextRenderer.Label("Game over", WIDTH / 2, HEIGHT / 2, SimpleText::CENTER);
    TextRenderer.Render();
}

void App::renderPause() {
    // Ecrit un texte noir centré à l'écran "Appuyez sur E pour commencer à jouer" en utilisant la police de taille 24
    TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::BLACK);
    TextRenderer.SetTextSize(SimpleText::SIZE_32);
    TextRenderer.Label("Appuyez sur P pour reprendre", WIDTH / 2, HEIGHT / 2, SimpleText::CENTER);
    TextRenderer.Render();
}


void App::update() {

    if (inMainMenu) {
        renderBackground();
        renderMenu();
        return;
    }

    if (game.isPaused) {
        renderBackground();
        render();
        renderPause();
        return;
    }

    if (game.map.gameOver) {
        renderBackground();
        renderGameOver();
        return;
    }

    const double currentTime { glfwGetTime() };
    const double elapsedTime { currentTime - _previousTime};
    game.map.elapsedTime = elapsedTime;

    _previousTime = currentTime;

    game.map.update();
    render();
}



void App::render() {



    // Clear the color and depth buffers of the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    renderBackground();
    game.render();

    TextRenderer.Render();
}

/*
 * CALLBACKS
 */
void App::key_callback(int key, int /*scancode*/, int action, int /*mods*/) {
    // Vérifier si la touche est "E" et l'action est un appui (GLFW_PRESS)
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        inMainMenu = false;
    }

    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        game.switchPause();
    }

    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
    }

    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        game.map.switchTowerMode();
    }
}


void App::mouse_button_callback(int button, int action, int /*mods*/) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (game.map.towerMode) {
            game.map.placeTower();
        }
    }
}

void App::scroll_callback(double /*xoffset*/, double /*yoffset*/) {
}

void App::cursor_position_callback(double xpos, double ypos) {
    // on a besoin de tracker uniquement si on est en mode construction de tour

    if (game.map.towerMode) {

        game.map.xCursor = xpos;
        game.map.yCursor = ypos;

        game.map.updateHoverSquare();

    }
}

void App::size_callback(int width, int height) {
    _width = width;
    _height = height;

    // make sure the viewport matches the new window dimensions
    glViewport(0, 0, _width, _height);

    // Change the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, _width, 0.0f, _height, -1.0f, 1.0f);
}

