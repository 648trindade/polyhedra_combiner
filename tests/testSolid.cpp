#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#define CATCH_CONFIG_CONSOLE_WIDTH 120
#include <catch2/catch.hpp>
#include <filesystem>

#include "../Solid.h"

Solid create_solid()
{
    Solid solid { "cube" };
    const Vertex* vertexes[8];
    EdgeInfo edges[12];

    size_t vertex_id = 0;
    for (float y : {-0.5, 0.5})
        for (float x : { -0.5, 0.5 })
            for (float z : {-0.5, 0.5})
                vertexes[vertex_id++] = solid.add_vertex(Vertex { x, y, z });

    auto add_face = [&solid, &vertexes](size_t a, size_t b, size_t c, size_t d) -> Vertex
    {
        Face& face = solid.add_face();
        EdgeInfo edge_ab = solid.add_edge(vertexes[a], vertexes[b]);
        EdgeInfo edge_bc = solid.add_edge(vertexes[b], vertexes[c]);
        EdgeInfo edge_cd = solid.add_edge(vertexes[c], vertexes[d]);
        EdgeInfo edge_da = solid.add_edge(vertexes[d], vertexes[a]);
        Contour& contour = face.add_contour();
        contour.add_edge(edge_ab);
        contour.add_edge(edge_bc);
        contour.add_edge(edge_cd);
        contour.add_edge(edge_da);
        face.compute_plane_equation();
        return face.normal;
    };

    Vertex normal;
    normal = add_face(0, 2, 3, 1);
    REQUIRE(normal == Vertex {0, -1, 0});
    normal = add_face(4, 5, 7, 6);
    REQUIRE(normal == Vertex {0, 1, 0});
    normal = add_face(0, 1, 5, 4);
    REQUIRE(normal == Vertex {-1, 0, 0});
    normal = add_face(2, 6, 7, 3);
    REQUIRE(normal == Vertex {1, 0, 0});
    normal = add_face(0, 4, 6, 2);
    REQUIRE(normal == Vertex {0, 0, -1});
    normal = add_face(3, 7, 5, 1);
    REQUIRE(normal == Vertex {0, 0, 1});

    return solid;
}

TEST_CASE("Solid is instantiated correctly")
{
    Solid solid {};
    REQUIRE(solid.faces.empty());

    REQUIRE(solid.vertexes.empty());
    auto vertex = solid.add_vertex(Vertex{0, 1, 2});
    REQUIRE(vertex);
    REQUIRE_FALSE(solid.vertexes.empty());

    REQUIRE(solid.edges.empty());
    auto edgeinfo = solid.add_edge(vertex, vertex);
    REQUIRE(edgeinfo.first);
    REQUIRE(edgeinfo.second);
}

TEST_CASE("Export to OBJ")
{
    Solid solid { create_solid() };
    std::string filename = "/tmp/testSolid.obj";
    solid.to_OBJ(filename);
    REQUIRE(std::filesystem::exists(filename));
    REQUIRE(std::filesystem::file_size(filename) > 0);
}