#include <catch2/catch.hpp>

#include "../Vertex.h"

#include <cfloat>
#include <sstream>
#include <string>

const float SIN_45 { float(std::sin(M_PI / 4.f)) };

TEST_CASE("Vertex is instantiated correctly")
{
    Vertex v { 0, 1, 2 };
    REQUIRE(v.x == 0);
    REQUIRE(v.y == 1);
    REQUIRE(v.z == 2);
}

TEST_CASE("Vertex equality")
{
    Vertex v { 0, 1, 2 };
    REQUIRE(v == v);
    REQUIRE_FALSE(v == (v + FLT_EPSILON));
}

TEST_CASE("Vertex addition")
{
    Vertex v { 0, 1, 2 };
    REQUIRE(v + v == Vertex { 0, 2, 4 });
    REQUIRE(v + 1 == Vertex { 1, 2, 3 });
    v += v;
    REQUIRE(v == Vertex { 0, 2, 4 });
    v += 1;
    REQUIRE(v == Vertex { 1, 3, 5 });
}

TEST_CASE("Vertex subtraction")
{
    Vertex v { 0, 1, 2 };
    REQUIRE(v - v == Vertex { 0, 0, 0 });
    REQUIRE(v - 1 == Vertex { -1, 0, 1 });
    v -= v;
    REQUIRE(v == Vertex { 0, 0, 0 });
    v -= 1;
    REQUIRE(v == Vertex { -1, -1, -1 });
}

TEST_CASE("Vertex multiplication")
{
    Vertex v { 0, 1, 2 };
    REQUIRE(v * v == Vertex { 0, 1, 4 });
    REQUIRE(v * 10 == Vertex { 0, 10, 20 });
    v *= v;
    REQUIRE(v == Vertex { 0, 1, 4 });
    v *= 10;
    REQUIRE(v == Vertex { 0, 10, 40 });
}

TEST_CASE("Vertex division")
{
    Vertex v { 1, 2, 3 };
    REQUIRE(v / v == Vertex { 1, 1, 1 });
    REQUIRE(v / 2 == Vertex { 0.5, 1, 1.5 });
    v /= v;
    REQUIRE(v == Vertex { 1, 1, 1 });
    v /= 2;
    REQUIRE(v == Vertex { 0.5, 0.5, 0.5 });
}

TEST_CASE("Vertex operator<<")
{
    Vertex v { 0, 1, 2 };
    std::stringstream os {};
    os << v;
    REQUIRE(os.str() == "(0, 1, 2)");
}

TEST_CASE("Vertex precedence operator")
{
    Vertex v { 0, 1, 2 };
    REQUIRE(v < Vertex { 3, 3, 3 });
    REQUIRE(v < Vertex { 3, 3, 0 });
    REQUIRE(v < Vertex { 3, 0, 0 });
}

TEST_CASE("Vertex dot")
{
    Vertex a { 1, 0, 0 };
    Vertex b { -1, 0, 0 };
    Vertex c { 0, 1, 0 };
    Vertex d = Vertex{ 1, 1, 0 }.normalize();
    REQUIRE(a.dot() == 1);
    REQUIRE(a.dot(b) == -1);
    REQUIRE(a.dot(c) == 0);
    REQUIRE(a.dot(d) == SIN_45);
}

TEST_CASE("Vertex cross")
{
    Vertex a { 1, 0, 0 };
    Vertex b { -1, 0, 0 };
    Vertex c { 0, 1, 0 };
    Vertex d { 0, -1, 0 };
    Vertex e { 0, 0, 1 };
    Vertex f { 0, 0, -1 };
    Vertex g = Vertex{ 1, 1, 0 }.normalize();
    REQUIRE(a.cross(a) == Vertex{0, 0, 0});
    REQUIRE(a.cross(b) == Vertex{0, 0, 0});
    REQUIRE(a.cross(c) == Vertex{0, 0, 1});
    REQUIRE(a.cross(d) == Vertex{0, 0, -1});
    REQUIRE(a.cross(e) == Vertex{0, -1, 0});
    REQUIRE(a.cross(f) == Vertex{0, 1, 0});
    REQUIRE(a.cross(g) == Vertex{0, 0, SIN_45});
}
