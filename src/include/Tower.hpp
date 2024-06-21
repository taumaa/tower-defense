#include "include/Square.hpp"
#include <vector>
#include <sil/sil.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>
#include "include/const.h"
#include "include/utils.hpp"
#include "include/GLHelpers.hpp"


class Tower {

public:

    Square* positionSquare;
    GLuint towerTexture {};

    void render();

    Tower(Square* positionSquare) {
        this->positionSquare = positionSquare;

        img::Image towerImg {img::load(make_absolute_path("images/tower.png", true), 3, false)};
        towerTexture = loadTexture(towerImg);
    }
};