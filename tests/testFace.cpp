#include <catch2/catch.hpp>

#include "../Face.h"

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