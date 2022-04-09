#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#define CATCH_CONFIG_CONSOLE_WIDTH 120
#include <catch2/catch.hpp>

#include "intersection.h"

// Forward declarations
void face_intersect(Face& face1, Face& face2);

TEST_CASE("Face Intersect")
{
    const Vertex vertexes[6] { { 0, 0, 0 },      { 0, 0, 1 },     { 1, 0, 0 },
                                { 0.1, -0.1, 0 }, { 0.1, 0.9, 0 }, { 0.1, -0.1, 1 } };
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

    face_intersect(a, b);
}