#include "include/utils.hpp"

#include <regex>
#include <filesystem>
#include <glad/glad.h>
#include <fstream>
#include <iostream>
#include <sil/sil.hpp>
#include <string>
#include <sstream>

std::filesystem::path make_absolute_path(std::filesystem::path const& path, bool check_path_exists)
{
    auto const res {path.is_relative() ? CMAKE_SOURCE_DIR / path : path };

    if (check_path_exists && !std::filesystem::exists(res))
    {
        using namespace std::literals;
        auto const msg { "Path \""s + res.string() + "\" does not exist. Make sure you didn't misspell it or made it relative to the wrong folder. All relative paths will be relative to the directory containing your CMakeLists.txt file." };
        std::cerr << msg << '\n';
        throw std::runtime_error{msg};
    }

    return res;
}

bool validateRGB(const std::string &line) {
    std::regex rgbRegex(R"((\d+)\s+(\d+)\s+(\d+))");
    std::smatch match;
    if (std::regex_search(line, match, rgbRegex)) {
        int r = std::stoi(match[1]);
        int g = std::stoi(match[2]);
        int b = std::stoi(match[3]);
        return r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255;
    }
    return false;
}

bool fileExists(const std::string &path) {
    return std::filesystem::exists(path);
}

void drawTexturedQuad(float x, float y, float width, float height, GLuint texture, bool flipVertical, bool hasBorder) {
    glPushMatrix();
    {
        // Positionner le carré
        glTranslatef(x, y, 0.0f);
        glBindTexture(GL_TEXTURE_2D, texture);
        glEnable(GL_TEXTURE_2D);

        // Dessiner le carré
        glColor3f(1.0f, 1.0f, 1.0f); // Blanc pour afficher la texture sans altération de couleur
        glBegin(GL_QUADS);
        if (flipVertical) {
            glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 0.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(width, 0.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(width, height);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, height);
        } else {
            glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(width, 0.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(width, height);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, height);
        }
        glEnd();

        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Dessiner la bordure si nécessaire
        if (hasBorder) {
            glColor3f(1.0f, 0.0f, 0.0f); // Rouge
            glLineWidth(2.0f); // Épaisseur de la bordure
            glBegin(GL_LINE_LOOP);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(width, 0.0f);
            glVertex2f(width, height);
            glVertex2f(0.0f, height);
            glEnd();
        }
    }
    glPopMatrix();
}

