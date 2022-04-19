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
    REQUIRE(face.distance == -2);
}

TEST_CASE("Edges intersect other face")
{
    const Vertex vertexes[6] { { 0, 0, 0 }, { 0, 0, 1 }, { 1, 0, 0 },
                               { 0.1, -0.1, 0.1 }, { 0.1, 0.5, 0.1 }, { 0.1, -0.1, 0.5 } };
    Face a {};
    a.add_vertex(vertexes[0]);
    a.add_vertex(vertexes[1]);
    a.add_vertex(vertexes[2]);
    a.compute_plane_equation();

    Face b {};
    b.add_vertex(vertexes[3]);
    b.add_vertex(vertexes[4]);
    b.add_vertex(vertexes[5]);
    b.compute_plane_equation();

    REQUIRE_FALSE(a.edges_intersect(b));
    REQUIRE(b.edges_intersect(a));
}

TEST_CASE("Get intersection line")
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
        auto [u, v] = a.get_intersection_line(b);
        REQUIRE(u.x == 0);
        REQUIRE(u.y == 0);
        REQUIRE(std::fabs(u.z) == 1);
        REQUIRE(v.x == 0);
        REQUIRE(v.y == 0);
    }
    {
        auto [u, v] = b.get_intersection_line(a);
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

    Face new_face { face.split(a, (b - a).normalize()) };
    REQUIRE(face.get_number_of_vertices() == 4);
    REQUIRE(face.vertices[0] == backup.vertices[0]);
    REQUIRE(face.vertices[1] == a);
    REQUIRE(face.vertices[2] == b);
    REQUIRE(face.vertices[3] == backup.vertices[2]);
    REQUIRE(new_face.get_number_of_vertices() == 3);
    REQUIRE(new_face.vertices[0] == a);
    REQUIRE(new_face.vertices[1] == backup.vertices[1]);
    REQUIRE(new_face.vertices[2] == b);

    // line on edges
    for (int i = 0; i < face.get_number_of_edges(); i++)
    {
        const Edge edge = face.get_edge(i);
        Face other_face = face.split(edge.start, edge.get_normal());
        REQUIRE(other_face.get_number_of_vertices() == 0);
        REQUIRE(face.get_number_of_vertices() == 4);
    }

    // line on other plane
    Face other_face = face.split(Vertex { 0, 0, 0 }, Vertex { 1, 0, 0 });
    REQUIRE(other_face.get_number_of_vertices() == 0);
    REQUIRE(face.get_number_of_vertices() == 4);
}

TEST_CASE("Faces are coplanar")
{
    const Vertex vertexes[6] { { 0, 0, 0 }, { 0, 0, 1 }, { 1, 0, 0 },
                               { 1, 0, 1 }, { 0, 0, 1 }, { 1, 0, 0 } };
    Face a {};
    a.add_vertex(vertexes[0]);
    a.add_vertex(vertexes[1]);
    a.add_vertex(vertexes[2]);
    a.compute_plane_equation();

    Face b {};
    b.add_vertex(vertexes[3]);
    b.add_vertex(vertexes[4]);
    b.add_vertex(vertexes[5]);
    b.compute_plane_equation();

    REQUIRE(a.is_coplanar(b));
    REQUIRE(b.is_coplanar(a));

    b.vertices[0].y += 1;
    b.compute_plane_equation();
    REQUIRE_FALSE(a.is_coplanar(b));
    REQUIRE_FALSE(b.is_coplanar(a));
}

TEST_CASE("Intersect coplanar face")
{
    const Vertex vertexes[6] { { 0, 0, 0 }, { 0, 0, 1 }, { 1, 0, 0 },
                               { 0, 0, 0 }, { 0, 0, 1 }, { 1, 0, 1 } };
    Face a {};
    a.add_vertex(vertexes[0]);
    a.add_vertex(vertexes[1]);
    a.add_vertex(vertexes[2]);
    a.compute_plane_equation();

    Face b {};
    b.add_vertex(vertexes[3]);
    b.add_vertex(vertexes[4]);
    b.add_vertex(vertexes[5]);
    b.compute_plane_equation();

    REQUIRE(a.intersect_coplanar(b));
    REQUIRE(b.intersect_coplanar(a));

    b.vertices[0].y += 1;
    b.compute_plane_equation();
    REQUIRE_FALSE(a.intersect_coplanar(b));
    REQUIRE_FALSE(b.intersect_coplanar(a));
}

TEST_CASE("Get intersection line - Precision bug #01")
{
    const Vertex vertexes[6] { { 0, 0, 0 }, { 1/3.f, 1, 1/3.f }, { 1, 0, 0 },
                               { 0, 1, 0 }, { 0, 1, 1 }, { 1, 1, 0 } };
    Face a {};
    a.add_vertex(vertexes[0]);
    a.add_vertex(vertexes[1]);
    a.add_vertex(vertexes[2]);
    a.compute_plane_equation();

    Face b {};
    b.add_vertex(vertexes[3]);
    b.add_vertex(vertexes[4]);
    b.add_vertex(vertexes[5]);
    b.compute_plane_equation();

    {
        auto [u, v] = a.get_intersection_line(b);
        REQUIRE(u == Vertex { 1, 0, 0 });
        REQUIRE(v == Vertex { 0, 1, 1/3.f});
    }
    {
        auto [u, v] = b.get_intersection_line(a);
        REQUIRE(u == Vertex { -1, 0, 0 });
        REQUIRE(v == Vertex { 0, 1, 1/3.f});
    }
}