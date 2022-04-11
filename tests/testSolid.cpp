#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#define CATCH_CONFIG_CONSOLE_WIDTH 120
#include <catch2/catch.hpp>

#include "Solid.h"

#include <filesystem>


Solid create_solid()
{
    Solid solid("cube");
    Vertex vertexes[8];

    size_t vertex_id = 0;
    for (float y : { -0.5, 0.5 })
        for (float x : { -0.5, 0.5 })
            for (float z : { -0.5, 0.5 })
                vertexes[vertex_id++] = Vertex { x, y, z };

    auto add_face = [&solid, &vertexes](size_t a, size_t b, size_t c, size_t d) -> Vertex {
        std::vector<Vertex> vertices { vertexes[a], vertexes[b], vertexes[c], vertexes[d] };
        Face& face = solid.add_face(vertices);
        return face.normal;
    };

    Vertex normal;
    normal = add_face(0, 2, 3, 1);
    REQUIRE(normal == Vertex { 0, -1, 0 });
    normal = add_face(4, 5, 7, 6);
    REQUIRE(normal == Vertex { 0, 1, 0 });
    normal = add_face(0, 1, 5, 4);
    REQUIRE(normal == Vertex { -1, 0, 0 });
    normal = add_face(2, 6, 7, 3);
    REQUIRE(normal == Vertex { 1, 0, 0 });
    normal = add_face(0, 4, 6, 2);
    REQUIRE(normal == Vertex { 0, 0, -1 });
    normal = add_face(3, 7, 5, 1);
    REQUIRE(normal == Vertex { 0, 0, 1 });

    return solid;
}

TEST_CASE("Solid is instantiated correctly")
{
    Solid solid {};

    REQUIRE(solid.faces.empty());
    solid.add_face();
    REQUIRE_FALSE(solid.faces.empty());

    auto& first_face = solid.faces[0];
    REQUIRE(first_face.get_number_of_vertices() == 0);
    first_face.add_vertex(Vertex { 0, 1, 2 });
    REQUIRE(first_face.get_number_of_vertices() == 1);

    REQUIRE(first_face.get_number_of_edges() == 0);
    first_face.add_vertex(Vertex { 2, 3, 4 });
    REQUIRE(first_face.get_number_of_edges() == 2);
}

TEST_CASE("Export to OBJ")
{
    Solid solid { create_solid() };
    std::string filename = "/tmp/testSolid.obj";
    solid.to_OBJ(filename);
    REQUIRE(std::filesystem::exists(filename));
    REQUIRE(std::filesystem::file_size(filename) > 0);
}