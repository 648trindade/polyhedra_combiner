#include "input_parser.h"
#include "intersection.h"

#include <iostream>
#include <vector>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <description.csv>" << std::endl;
        return 1;
    }
    std::vector<Solid> solids;

    auto input_geometries = read_input(argv[1]);
    for (const auto& input_geometry : input_geometries)
    {
        solids.emplace_back(setup_geometry(input_geometry));
    }

    remove_intersect(solids);

    return 0;
}