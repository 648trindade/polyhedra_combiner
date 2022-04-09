#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#define CATCH_CONFIG_CONSOLE_WIDTH 120
#include <catch2/catch.hpp>

#include "input_parser.h"

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
        REQUIRE(solid.faces.size() == 12);

        for (auto& face: solid.faces)
        {
            // check faces meta info
            REQUIRE(face.get_number_of_vertices() == 3);
            REQUIRE(face.get_number_of_edges() == 3);
            {
                auto a = face.vertices[0];
                auto b = face.vertices[1];
                auto c = face.vertices[2];
                REQUIRE(face.normal == (b - a).cross(c - a).normalize());
            }

            // check closed contours
            for (int i = 0; i < face.get_number_of_edges(); i++)
            {
                int next = (i + 1) % face.get_number_of_edges();
                Vertex a = face.get_edge(i).end;
                Vertex b = face.get_edge(next).start;
                REQUIRE(a == b);
            }
        }
        solid.to_OBJ("/tmp/" + solid.name + ".obj");
    }
}