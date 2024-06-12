#include "include/utils.hpp"

#include <iostream>
#include <string>
#include <regex>
#include <filesystem>

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