#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#define CATCH_CONFIG_CONSOLE_WIDTH 120
#include <catch2/catch.hpp>

#include "../input_parser.h"

#include <iostream>

TEST_CASE("Two cubes")
{
    auto result = read_input("tests/assets/two_cubes.json");
    REQUIRE(result.size() == 2);
    REQUIRE(result[0].path.filename() == "cube.obj");
    REQUIRE(result[0].center == Vertex{});
    REQUIRE(result[1].path.filename() == "cube.obj");
    REQUIRE(result[1].center == Vertex { 1, 0, 0 });

    for (auto& geometry: result)
    {
        auto solid = setup_geometry(geometry);
        REQUIRE(solid.vertexes.size() == 8);
        REQUIRE(solid.edges.size() == 18);
        REQUIRE(solid.faces.size() == 12);

        for (auto& face: solid.faces)
        {
            // check faces meta info
            REQUIRE(face.get_number_of_vertexes() == 3);
            {
                auto a = face.get_vertex(0);
                auto b = face.get_vertex(1);
                auto c = face.get_vertex(2);
                REQUIRE(face.normal == (*b - *a).cross(*c - *a).normalize());
            }

            // check closed contours
            for (auto& contour : face.contours)
            {
                for (auto it = contour.edges.begin(); it != contour.edges.end(); it++)
                {
                    auto next = (std::next(it) != contour.edges.end()) ? std::next(it)
                                                                       : contour.edges.begin();
                    auto a = (it->second) ? it->first->end : it->first->start;
                    auto b = (next->second) ? next->first->start : next->first->end;
                    REQUIRE(*a == *b);
                }
            }
        }
    }
}