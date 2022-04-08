#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#define CATCH_CONFIG_CONSOLE_WIDTH 120
#include <catch2/catch.hpp>

#include "../input_parser.h"
#include "../intersection.h"

// Forward declarations
void face_intersect(Face& face1, Face& face2);

TEST_CASE("Face Intersect")
{
    const Vertex _vertexes[6] { { 0, 0, 0 },      { 0, 0, 1 },     { 1, 0, 0 },
                                { 0.1, -0.1, 0 }, { 0.1, 0.9, 0 }, { 0.1, -0.1, 1 } };
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
    }
    Face b {};
    {
        Contour& contour = b.add_contour();
        contour.add_edge(std::make_pair(&_edges[3], true));
        contour.add_edge(std::make_pair(&_edges[4], true));
        contour.add_edge(std::make_pair(&_edges[5], true));
        b.compute_plane_equation();
    }

    face_intersect(a, b);
}