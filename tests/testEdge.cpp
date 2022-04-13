#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#define CATCH_CONFIG_CONSOLE_WIDTH 120
#include <catch2/catch.hpp>
#include <numeric.h>

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

TEST_CASE("Edge intersect line")
{
    const Edge a { Vertex { 0, 0, 0 }, Vertex { 0, 3, 0 } };
    // parallel line
    for (float x : {-1.f, 1.f})
    {
        const Vertex line_point { x, 1, 0 };
        const Vertex line_vector { -x, 0, 0 };
        auto [intersect, point] = a.intersect(line_vector, line_point);
        REQUIRE(intersect);
        REQUIRE(point == Vertex { 0, 1, 0 });
    }
    // parallel line
    for (float x : {-1.f, 1.f})
    {
        const Vertex line_point { x, 1, 0 };
        const Vertex line_vector { x, 0, 0 };
        auto [intersect, point] = a.intersect(line_vector, line_point);
        REQUIRE(intersect);
        REQUIRE(point == Vertex { 0, 1, 0 });
    }
    // 90º degree line
    for (float x : { -1.f, 1.f })
    {
        for (bool y : { true, false })
        {
            const Vertex line_point { x, (y ? 0.f : 3.f), 0 };
            const Vertex line_vector { -2 * x, (y ? 3.f : -3.f), 0 };
            auto [intersect, point] = a.intersect(line_vector.normalize(), line_point);
            REQUIRE(intersect);
            REQUIRE(point == Vertex { 0, 1.5f, 0 });
        }
    }
    // 90º+ degree line
    for (float x : { -1.f, 1.f })
    {
        for (bool y : { true, false })
        {
            const Vertex line_point { x, (y ? -1.f : 4.f), 0 };
            const Vertex line_vector { -2 * x, (y ? 5.f : -5.f), 0 };
            auto [intersect, point] = a.intersect(line_vector.normalize(), line_point);
            REQUIRE(intersect);
            REQUIRE(point == Vertex { 0, 1.5f, 0 });
        }
    }
    // non intersecting parallel line
    {
        const Vertex line_point { 1, 0, 0 };
        const Vertex line_vector { 0, 1, 0 };
        auto [intersect, point] = a.intersect(line_vector.normalize(), line_point);
        REQUIRE_FALSE(intersect);
    }
    // non intersecting line from other plane
    {
        const Vertex line_point { -1, 1, 2 };
        const Vertex line_vector { 1, 0, 2 };
        auto [intersect, point] = a.intersect(line_vector, line_point);
        REQUIRE_FALSE(intersect);
    }
}

TEST_CASE("Edge intersect edge")
{
    const Edge a { Vertex { 0, 0, 0 }, Vertex { 0, 3, 0 } };
    const Edge b { Vertex { -1, 1, 0 }, Vertex { 1, 1, 0 } };
    {
        auto [intersect, point] = a.intersect(b);
        REQUIRE(intersect);
        REQUIRE(point == Vertex { 0, 1, 0 });
    }
    {
        auto [intersect, point] = b.intersect(a);
        REQUIRE(intersect);
        REQUIRE(point == Vertex { 0, 1, 0 });
    }
    const Edge c { Vertex { -1, 1, 0 }, Vertex { -0.1, 1, 0 } };
    {
        auto [intersect, point] = a.intersect(c);
        REQUIRE_FALSE(intersect);
    }
    {
        auto [intersect, point] = c.intersect(a);
        REQUIRE_FALSE(intersect);
    }
    const Edge d { Vertex { 1, 1, 0 }, Vertex { 1.1, 1, 0 } };
    {
        auto [intersect, point] = a.intersect(c);
        REQUIRE_FALSE(intersect);
    }
    {
        auto [intersect, point] = c.intersect(a);
        REQUIRE_FALSE(intersect);
    }
}