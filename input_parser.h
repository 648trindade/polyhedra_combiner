#pragma once

#include "Solid.h"

#include <filesystem>
#include <string>
#include <vector>

struct InputGeometry
{
    std::filesystem::path path;
    float scale = 1.f;
    Vertex center { 0, 0, 0 };
    float rotation_matrix[3][3] { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };
};

std::vector<InputGeometry> read_input(const std::string& JSON_filename);

Solid setup_geometry(const InputGeometry& geometry_info);