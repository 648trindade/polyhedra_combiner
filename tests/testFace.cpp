#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#define CATCH_CONFIG_CONSOLE_WIDTH 120
#include <catch2/catch.hpp>

#include "Face.h"

Face create_face()
{
    std::vector<Vertex> vertexes { { 0, 1, 2 }, { 1, 1, 2 }, { 1, 0, 2 } };
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
    REQUIRE(face.normal == Vertex { 0, 0, -1 });
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

TEST_CASE("Intersect Point")
{
    Face face { create_face() };
    REQUIRE(face.intersect(Vertex { 0.75, 0.75, 2 })); // inside
    REQUIRE_FALSE(face.intersect(Vertex { 0, 0, 2 })); // origin
    REQUIRE_FALSE(face.intersect(Vertex { 1 + numeric::epsilon(), 1, 2 })); // edge +
    REQUIRE_FALSE(face.intersect(Vertex { 0 - numeric::epsilon(), 1, 2 })); // edge -
    REQUIRE(face.intersect(Vertex { 0, 1, 2 })); // edge start
    REQUIRE_FALSE(face.intersect(Vertex { 0.75, 0.75, 0 })); // below
    REQUIRE_FALSE(face.intersect(Vertex { 0.75, 0.75, 3 })); // above
}

TEST_CASE("Face Split")
{
    Face face { create_face() };
    Face backup = face;
    Vertex a { (face.vertices[0] + face.vertices[1]) / 2 };
    Vertex b { (face.vertices[1] + face.vertices[2]) / 2 };
    Face new_face { face.split(0, a, 1, b) };
    REQUIRE(face.get_number_of_vertices() == 4);
    REQUIRE(face.vertices[0] == backup.vertices[0]);
    REQUIRE(face.vertices[1] == a);
    REQUIRE(face.vertices[2] == b);
    REQUIRE(face.vertices[3] == backup.vertices[2]);
    REQUIRE(new_face.get_number_of_vertices() == 3);
    REQUIRE(new_face.vertices[0] == a);
    REQUIRE(new_face.vertices[1] == backup.vertices[1]);
    REQUIRE(new_face.vertices[2] == b);
    REQUIRE_THROWS(backup.split(0, Vertex {}, 1, Vertex {}));
}