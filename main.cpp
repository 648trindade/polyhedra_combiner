#include "Vertex.h"
#include "input_parser.h"

#include "OBJ_Loader.h"

#include <iostream>
#include <stdexcept>
#include <vector>

int main(int argc, char* argv[])
{
    objl::Loader loader;
    std::vector<Vertex> vertices;

    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <description.csv>" << std::endl;
        return 1;
    }
    auto input_geometries = read_input(argv[1]);

    for (const auto& input_geometry : input_geometries)
    {
        bool ok = loader.LoadFile(input_geometry.path.string());
        if (!ok)
        {
            throw std::runtime_error("Fail to load " + input_geometry.path.string());
        }

        std::cout << loader.LoadedVertices.size() << " vertexes" << std::endl;
        std::cout << loader.LoadedIndices.size() << " indexes" << std::endl;
    }
}