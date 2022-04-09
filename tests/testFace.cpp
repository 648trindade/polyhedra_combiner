#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#define CATCH_CONFIG_CONSOLE_WIDTH 120
#include <catch2/catch.hpp>

#include "../Face.h"

Face create_face()
{
    std::vector<Vertex> vertexes { { 1, 0, 2 }, { 1, 1, 2 }, { 0, 1, 2 } };
    return { vertexes };
}

TEST_CASE("Face is instantiated correctly")
{
    Face face {};
    REQUIRE(face.vertices.empty());
    REQUIRE(face.distance == 0);
    REQUIRE(face.normal == Vertex {});
}

TEST_CASE("Compute plane equation")
{
    Face face = create_face();
    face.compute_plane_equation();
    REQUIRE(face.normal == Vertex { 0, 0, 1 });
    REQUIRE(face.distance == 2);
}

TEST_CASE("Intersect faces - origin")
{
    const Vertex vertexes[6] { { 0, 0, 0 }, { 0, 0, 1 }, { 1, 0, 0 },
                               { 0, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };
    Face a {};
    a.add_vertex(vertexes[0]);
    a.add_vertex(vertexes[1]);
    a.add_vertex(vertexes[2]);
    a.compute_plane_equation();
    REQUIRE(a.normal == Vertex { 0, 1, 0 });
    REQUIRE(a.distance == 0);

    Face b {};
    b.add_vertex(vertexes[3]);
    b.add_vertex(vertexes[4]);
    b.add_vertex(vertexes[5]);
    b.compute_plane_equation();
    REQUIRE(b.normal == Vertex { 1, 0, 0 });
    REQUIRE(b.distance == 0);

    {
        auto [u, v] = a.intersect(b);
        REQUIRE(u.x == 0);
        REQUIRE(u.y == 0);
        REQUIRE(std::fabs(u.z) == 1);
        REQUIRE(v.x == 0);
        REQUIRE(v.y == 0);
    }
    {
        auto [u, v] = b.intersect(a);
        REQUIRE(u.x == 0);
        REQUIRE(u.y == 0);
        REQUIRE(std::fabs(u.z) == 1);
        REQUIRE(v.x == 0);
        REQUIRE(v.y == 0);
    }
}

TEST_CASE("Intersect edge")
{
    Face face { create_face() };
    Vertex start { 0.75, 0.75, 1 };
    Vertex end { 0.75, 0.75, 3 };
    {
        Edge edge { start, end };
        auto [intersect, point] = face.intersect(edge);
        REQUIRE(intersect);
        REQUIRE(point == Vertex { 0.75, 0.75, 2 });
    }
    {
        start.y = 1;
        end.y = 0;
        Edge edge { start, end };
        auto [intersect, point] = face.intersect(edge);
        REQUIRE(intersect);
        REQUIRE(point.x == 0.75);
        REQUIRE(point.z == 2);
        REQUIRE((end - point).normalize() == edge.get_normal());
    }
    {
        end.z = 1.5;
        Edge edge { start, end };
        auto [intersect, point] = face.intersect(edge);
        REQUIRE_FALSE(intersect);
        REQUIRE(point == Vertex {});
    }
}