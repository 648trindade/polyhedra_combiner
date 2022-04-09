#include "input_parser.h"

#include "json.hpp"

#include <OBJ_Loader.h>
#include <fstream>

std::vector<InputGeometry> read_input(const std::string& JSON_filename)
{
    std::vector<InputGeometry> result;
    nlohmann::json json;

    if (!std::filesystem::exists(JSON_filename))
    {
        throw std::runtime_error("JSON config file doesn't exist: " + JSON_filename);
    }

    try
    {
        std::ifstream file(JSON_filename);
        file >> json;
        file.close();
    }
    catch (nlohmann::json::parse_error& error)
    {
        throw std::runtime_error("Error while parsing " + JSON_filename + ":\n" + error.what());
    }

    for (auto entry : json)
    {
        if (!entry.contains("path"))
        {
            throw std::runtime_error("You should provide a OBJ path for all geometry entries");
        }

        InputGeometry geometry;
        std::string path = entry["path"].get<std::string>();
        geometry.path = std::filesystem::path(path);
        if (!std::filesystem::exists(geometry.path))
        {
            throw std::runtime_error("Inexistent OBJ file: " + path);
        }
        if (entry.contains("center"))
        {
            auto json_center = entry["center"];
            geometry.center = Vertex {
                json_center[0].get<float>(),
                json_center[1].get<float>(),
                json_center[2].get<float>()
            };
        }
        if (entry.contains("scale"))
        {
            geometry.scale = entry["scale"].get<float>();
        }
        if (entry.contains("quaternion"))
        {
            auto json_quaternion = entry["center"];
            const float q[4] = {
                json_quaternion[0].get<float>(), // scalar
                json_quaternion[1].get<float>(),
                json_quaternion[2].get<float>(),
                json_quaternion[3].get<float>()
            };
            geometry.rotation_matrix[0][0] = 2 * (q[0] * q[0] + q[1] * q[1]) - 1;
            geometry.rotation_matrix[0][1] = 2 * (q[1] * q[2] - q[0] * q[3]);
            geometry.rotation_matrix[0][2] = 2 * (q[1] * q[3] + q[0] * q[2]);
            geometry.rotation_matrix[1][0] = 2 * (q[1] * q[2] + q[0] * q[3]);
            geometry.rotation_matrix[1][1] = 2 * (q[0] * q[0] + q[2] * q[2]) - 1;
            geometry.rotation_matrix[1][2] = 2 * (q[2] * q[3] - q[0] * q[1]);
            geometry.rotation_matrix[2][0] = 2 * (q[1] * q[3] - q[0] * q[2]);
            geometry.rotation_matrix[2][1] = 2 * (q[2] * q[3] + q[0] * q[1]);
            geometry.rotation_matrix[2][2] = 2 * (q[0] * q[0] + q[3] * q[3]) - 1;
        }

        result.push_back(geometry);
    }

    return result;
}

Solid setup_geometry(const InputGeometry& geometry_info)
{
    objl::Loader loader;
    bool ok = loader.LoadFile(geometry_info.path.string());
    if (!ok)
    {
        throw std::runtime_error("Fail to load " + geometry_info.path.string());
    }

    auto transform_vertex = [&geometry_info](Vertex& vertex)
    {
        vertex *= geometry_info.scale;
        vertex.rotate(geometry_info.rotation_matrix);
        vertex += geometry_info.center;
    };

    Solid solid {};
    solid.name = geometry_info.path.stem();
    for (size_t i = 0; i < loader.LoadedIndices.size(); i += 3)
    {
        std::vector<Vertex> vertices(3);
        for (int v = 0; v < 3; v++)
        {
            auto index = loader.LoadedIndices[i + v];
            auto& position = loader.LoadedVertices[index].Position;
            Vertex vertex { position.X, position.Y, position.Z };
            transform_vertex(vertex);
            vertices[v] = vertex;
        }
        solid.add_face(vertices);
    }
    return solid;
}
