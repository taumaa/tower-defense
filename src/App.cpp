#include "include/App.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>

#include <sstream>
#include <iostream>

#include "simpletext.h"
#include "include/utils.hpp"
#include "include/GLHelpers.hpp"

App::App() : _previousTime(0.0), _viewSize(2.0) {
   // load what needs to be loaded here (for example textures)

    img::Image test {img::load(make_absolute_path("images/level.png", true), 3, true)};
    
    _texture = loadTexture(test);
}

void App::setup() {
    // Set the clear color to a nice blue
    glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

    // Setup the text renderer with blending enabled and white text color
    TextRenderer.ResetFont();
    TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::WHITE);
    TextRenderer.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 0.f, 0.f);
    TextRenderer.EnableBlending(true);

    // Test
    game.map.setup("maps/map.png", "maps/map.itd");
    std::cout << "Setup called" << std::endl;



    game.map.test();

}

void App::update() {

    const double currentTime { glfwGetTime() };
    const double elapsedTime { currentTime - _previousTime};
    _previousTime = currentTime;

    _angle += 10.0f * elapsedTime;
    // _angle = std::fmod(_angle, 360.0f);
    
    render();
}

void App::renderMenu() {
    if (game.isInMenu) {
        TextRenderer.Label("We are in menu", _width / 2, 20, SimpleText::CENTER);
    } else {
        TextRenderer.Label("We are in game", _width / 2, 20, SimpleText::CENTER);
    }
}

void App::render() {
    // Clear the color and depth buffers of the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    renderMenu();
    game.map.render();

    TextRenderer.Render();
}

/*
 * CALLBACKS
 */
void App::key_callback(int key, int /*scancode*/, int action, int /*mods*/) {
    // Vérifier si la touche est "E" et l'action est un appui (GLFW_PRESS)
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        game.isInMenu = !game.isInMenu;
    }
}


void App::mouse_button_callback(int /*button*/, int /*action*/, int /*mods*/) {
}

void App::scroll_callback(double /*xoffset*/, double /*yoffset*/) {
}

void App::cursor_position_callback(double /*xpos*/, double /*ypos*/) {
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

