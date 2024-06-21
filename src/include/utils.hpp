#pragma once
#include <filesystem>
#include <glm/glm.hpp>


#include <glad/glad.h>
#include <fstream>
#include <iostream>
#include <sil/sil.hpp>
#include <string>
#include <sstream>

std::filesystem::path make_absolute_path(std::filesystem::path const& path, bool check_path_exists = true);
void drawTexturedQuad(float x, float y, float width, float height, GLuint texture, bool flipVertical = false, bool hasBorder = false);

template <glm::length_t N, typename T, glm::qualifier Q>
std::ostream& operator<<(std::ostream& os,glm::vec<N, T, Q> const& vec) {
    os << "(";
    
    os << vec[0];
    for (size_t i {1}; i < N; ++i) {
        os << ", " << vec[i];
    }
    os << ")";
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> const& vec)
{
    if (vec.empty())
    {
        return os << "[]";
    }

    os << '[';
    for (size_t i {0}; i < vec.size() - 1; ++i)
    {
        os << vec[i] << ", ";
    }
    return os << vec.back() << ']';
}

bool validateRGB(const std::string &line);
bool fileExists(const std::string &path);