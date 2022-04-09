#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#define CATCH_CONFIG_CONSOLE_WIDTH 120
#include <catch2/catch.hpp>

#include "Edge.h"

TEST_CASE("Edge direction")
{
    const Vertex a { 0, 0, 0 }, b { 2, 0, 0 };
    const Edge e { a, b };
    REQUIRE(e.get_direction() == b);
}

TEST_CASE("Edge normal")
{
    const Vertex a { 0, 0, 0 }, b { 2, 0, 0 };
    const Edge e { a, b };
    REQUIRE(e.get_normal() == Vertex { 1, 0, 0 });
}

TEST_CASE("Edge length")
{
    const Vertex a { 0, 0, 0 }, b { 2, 0, 0 };
    const Edge e { a, b };
    REQUIRE(e.get_length() == 2);
}

TEST_CASE("Edge intersect point")
{
    const Vertex a { 0, 0, 0 }, b { 2, 0, 0 };
    const Edge e { a, b };
    const Vertex p { 1, 0, 0 }, q { -1, 0, 0 }, r { 3, 0, 0 }, s { 1, 1, 1 };
    REQUIRE(e.intersect(a));
    REQUIRE(e.intersect(b));
    REQUIRE(e.intersect(p));
    REQUIRE_FALSE(e.intersect(q));
    REQUIRE_FALSE(e.intersect(r));
    REQUIRE_FALSE(e.intersect(s));
}