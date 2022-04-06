#include <catch2/catch.hpp>

#include "../BoundingBox.h"

TEST_CASE("Empty Bounding Box")
{
    BoundingBox box {};
    REQUIRE(
        box.max
        == Vertex { Vertex::limits::lowest(), Vertex::limits::lowest(), Vertex::limits::lowest() });
    REQUIRE(
        box.min == Vertex { Vertex::limits::max(), Vertex::limits::max(), Vertex::limits::max() });
}

TEST_CASE("Update Bounding Box")
{
    BoundingBox box {};
    {
        Vertex v { 0, -1, 1 };
        box.update(v);
        REQUIRE(box.max == v);
        REQUIRE(box.min == v);
    }
    {
        Vertex v { -1, 1, -1 };
        box.update(v);
        REQUIRE(box.max == Vertex { 0, 1, 1 });
        REQUIRE(box.min == Vertex { -1, -1, -1 });
    }
}

TEST_CASE("Bounding Box Sides and Volume")
{
    BoundingBox box{};
    box.update(Vertex { 0.5, 1.0, 1.5 });
    box.update(Vertex { -0.5, -1.0, -1.5 });

    REQUIRE(box.dx() == 1.0);
    REQUIRE(box.dy() == 2.0);
    REQUIRE(box.dz() == 3.0);
    REQUIRE(box.volume() == 6.0);
}

TEST_CASE("Merge Bounding Boxes")
{
    BoundingBox box_a {}, box_b {};
    box_a.update(Vertex {});
    box_a.update(Vertex { 1, 1, 1 });
    box_b.update(Vertex {});
    box_b.update(Vertex { -1, -1, -1 });

    BoundingBox merged = box_a.merge(box_b);
    REQUIRE(merged.max == box_a.max);
    REQUIRE(merged.min == box_b.min);
}

TEST_CASE("Overlap Boxes")
{
    BoundingBox box_a {}, box_b {};
    box_a.update(Vertex { 0, 0, Vertex::limits::epsilon() });
    box_a.update(Vertex { 1, 1, 1 });
    box_b.update(Vertex {});
    box_b.update(Vertex { -1, -1, -1 });
    REQUIRE_FALSE(box_a.overlap(box_b));
    REQUIRE_FALSE(box_b.overlap(box_a));

    box_a.update(Vertex { -0.5, -0.5, -0.5 });
    REQUIRE(box_a.overlap(box_b));
    REQUIRE(box_b.overlap(box_a));
}

TEST_CASE("Overlap Point")
{
    BoundingBox box {};
    box.update(Vertex {});
    box.update(Vertex { 1, 1, 1 });

    REQUIRE(box.overlap(Vertex { 0.5, 0.5, 0.5 }));
    REQUIRE(box.overlap(Vertex { 0, 0, 0 }));
    REQUIRE(box.overlap(Vertex { 1, 0, 0 }));
    REQUIRE(box.overlap(Vertex { 0, 1, 0 }));
    REQUIRE(box.overlap(Vertex { 0, 0, 1 }));
    REQUIRE_FALSE(box.overlap(Vertex { -1, 0, 0 }));
    REQUIRE_FALSE(box.overlap(Vertex { 2, 0, 0 }));
    REQUIRE_FALSE(box.overlap(Vertex { 0, -1, 0 }));
    REQUIRE_FALSE(box.overlap(Vertex { 0, 2, 0 }));
    REQUIRE_FALSE(box.overlap(Vertex { 0, 0, -1 }));
    REQUIRE_FALSE(box.overlap(Vertex { 0, 0, 2 }));
}