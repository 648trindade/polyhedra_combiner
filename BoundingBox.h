#include "Vertex.h"

#include <limits>

#pragma once

class BoundingBox
{
public:
    Vertex min { std::numeric_limits<float>::max(),
                 std::numeric_limits<float>::max(),
                 std::numeric_limits<float>::max() };
    Vertex max { std::numeric_limits<float>::min(),
                 std::numeric_limits<float>::min(),
                 std::numeric_limits<float>::min() };

    void update(Vertex vertex);
    bool overlap(const BoundingBox& other) const;
};
