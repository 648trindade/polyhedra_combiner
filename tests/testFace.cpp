#include <catch2/catch.hpp>

#include "../Face.h"

#include <iostream>

const Vertex vertexes[3] {{1, 0, 2}, {1, 1, 2}, {0, 1, 2}};
const Edge edges[3] {{vertexes, vertexes + 1}, {vertexes + 1, vertexes + 2}, {vertexes + 2, vertexes}};

Face create_face()
{
    Face face{};
    Contour& contour = face.add_contour();
    contour.add_edge(std::make_pair(&edges[0], true));
    contour.add_edge(std::make_pair(&edges[1], true));
    contour.add_edge(std::make_pair(&edges[2], true));
    return face;
}

TEST_CASE("Face is instantiated correctly")
{
    Face face{};
    REQUIRE(face.contours.empty());
    REQUIRE(face.distance == 0);
    REQUIRE(face.normal == Vertex{});
}

TEST_CASE("Add Contour")
{
    Face face{};
    Contour& contour = face.add_contour();
    REQUIRE(face.contours.size() == 1);
    REQUIRE(&contour == face.contours.data());
    REQUIRE(contour.edges.empty());
}

TEST_CASE("Compute plane equation")
{
    Face face = create_face();
    face.compute_plane_equation();
    REQUIRE(face.normal == Vertex{0, 0, 1});
    REQUIRE(face.distance == 2);
}

TEST_CASE("Intersect faces - origin")
{
    const Vertex _vertexes[6] { { 0, 0, 0 }, { 0, 0, 1 }, { 1, 0, 0 },
                                { 0, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };
    const Edge _edges[6] { { _vertexes, _vertexes + 1 },     { _vertexes + 1, _vertexes + 2 },
                           { _vertexes + 2, _vertexes },     { _vertexes + 3, _vertexes + 4 },
                           { _vertexes + 4, _vertexes + 5 }, { _vertexes + 5, _vertexes + 3 } };

    Face a {};
    {
        Contour& contour = a.add_contour();
        contour.add_edge(std::make_pair(&_edges[0], true));
        contour.add_edge(std::make_pair(&_edges[1], true));
        contour.add_edge(std::make_pair(&_edges[2], true));
        a.compute_plane_equation();
        REQUIRE(a.normal == Vertex { 0, 1, 0 });
        REQUIRE(a.distance == 0);
    }
    Face b {};
    {
        Contour& contour = b.add_contour();
        contour.add_edge(std::make_pair(&_edges[3], true));
        contour.add_edge(std::make_pair(&_edges[4], true));
        contour.add_edge(std::make_pair(&_edges[5], true));
        b.compute_plane_equation();
        REQUIRE(b.normal == Vertex { 1, 0, 0 });
        REQUIRE(b.distance == 0);
    }
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