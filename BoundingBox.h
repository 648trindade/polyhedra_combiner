#pragma once

#include "Vertex.h"

#include <limits>

class BoundingBox
{
public:
    Vertex min { Vertex::limits::max(), Vertex::limits::max(), Vertex::limits::max() };
    Vertex max { Vertex::limits::lowest(), Vertex::limits::lowest(), Vertex::limits::lowest() };

    void update(const Vertex& vertex);
    bool overlap(const BoundingBox& other) const;
    bool overlap(const Vertex& other) const;
    BoundingBox merge(const BoundingBox& other) const;

    float dx() const;
    float dy() const;
    float dz() const;
    float volume() const;
};
