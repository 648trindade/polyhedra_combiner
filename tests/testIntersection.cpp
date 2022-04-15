#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#define CATCH_CONFIG_CONSOLE_WIDTH 120
#include <catch2/catch.hpp>

#include "intersection.h"

TEST_CASE("Face Intersect")
{
    const Vertex vertexes[6] { { 0, 0, 0 },        { 0, 0, 1 },       { 1, 0, 0 },
                               { 0.1, -0.1, 0.1 }, { 0.1, 0.5, 0.1 }, { 0.1, -0.1, 0.5 } };
    Face home_face {};
    home_face.add_vertex(vertexes[0]);
    home_face.add_vertex(vertexes[1]);
    home_face.add_vertex(vertexes[2]);
    home_face.compute_plane_equation();

    Face near_face {};
    near_face.add_vertex(vertexes[3]);
    near_face.add_vertex(vertexes[4]);
    near_face.add_vertex(vertexes[5]);
    near_face.compute_plane_equation();

    const Vertex home_cut_0 { 0.1, 0, 0.9 };
    const Vertex home_cut_1 { 0.1, 0, 0 };
    const Vertex near_cut_0 { 0.1, 0, 0.1 };
    const Vertex near_cut_1 { 0.1, 0, 13.0 / 30.0 };

    auto [intersect, new_home_face, new_near_face] = face_intersect(home_face, near_face);
    REQUIRE(intersect);
    REQUIRE(home_face.get_number_of_vertices() == 4);
    REQUIRE(home_face.vertices[0] == vertexes[0]);
    REQUIRE(home_face.vertices[1] == vertexes[1]);
    REQUIRE(home_face.vertices[2] == home_cut_0);
    REQUIRE(home_face.vertices[3] == home_cut_1);
    REQUIRE(new_home_face.get_number_of_vertices() == 3);
    REQUIRE(new_home_face.vertices[0] == home_cut_0);
    REQUIRE(new_home_face.vertices[1] == vertexes[2]);
    REQUIRE(new_home_face.vertices[2] == home_cut_1);
    REQUIRE(near_face.get_number_of_vertices() == 4);
    REQUIRE(near_face.vertices[0] == vertexes[3]);
    REQUIRE(near_face.vertices[1] == near_cut_0);
    REQUIRE(near_face.vertices[2] == near_cut_1);
    REQUIRE(near_face.vertices[3] == vertexes[5]);
    REQUIRE(new_near_face.get_number_of_vertices() == 3);
    REQUIRE(new_near_face.vertices[0] == near_cut_0);
    REQUIRE(new_near_face.vertices[1] == vertexes[4]);
    REQUIRE(new_near_face.vertices[2] == near_cut_1);
}

TEST_CASE("Draw new surfaces")
{
    const Vertex vertices[8] { Vertex { 0, 0, 0 }, Vertex { 1, 0, 0 }, Vertex { 0, 0, 1 },
                               Vertex { 0, 1, 0 }, Vertex { 0, 1, 1 }, Vertex { 1, 1, 0 }};
    const Vertex peaks[2] { ((vertices[0] + vertices[1] + vertices[2]) / 3.0) + Vertex { 0, 1, 0 },
                            ((vertices[3] + vertices[4] + vertices[5]) / 3.0) - Vertex { 0, 1, 0 } };
    Solid a { "Tetra_up" };
    a.add_face(std::vector<Vertex> { vertices[0], vertices[1], vertices[2] });
    a.add_face(std::vector<Vertex> { vertices[0], peaks[0], vertices[1] });
    a.add_face(std::vector<Vertex> { vertices[0], vertices[2], peaks[0] });
    a.add_face(std::vector<Vertex> { vertices[1], peaks[0], vertices[2] });
    Solid b { "Tetra_down" };
    b.add_face(std::vector<Vertex> { vertices[3], vertices[4], vertices[5] });
    b.add_face(std::vector<Vertex> { vertices[3], vertices[5], peaks[1] });
    b.add_face(std::vector<Vertex> { vertices[3], peaks[1], vertices[4] });
    b.add_face(std::vector<Vertex> { vertices[5], vertices[4], peaks[1] });

    intersect(a, b);
    draw_new_surfaces(a, b);
    a.to_OBJ("/tmp/Tetra_up.obj");
    b.to_OBJ("/tmp/Tetra_down.obj");
}