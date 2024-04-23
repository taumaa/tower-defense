#pragma once
#include <filesystem>

std::filesystem::path make_absolute_path(std::filesystem::path const& path, bool check_path_exists = true);