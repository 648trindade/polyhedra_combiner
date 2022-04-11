#pragma once

#include "Vertex.h"
#include "numeric.h"

class BoundingBox
{
public:
    Vertex min { numeric::max(), numeric::max(), numeric::max() };
    Vertex max { numeric::lowest(), numeric::lowest(), numeric::lowest() };

    void update(const Vertex& vertex);
    bool overlap(const BoundingBox& other) const;
    bool overlap(const Vertex& other) const;
    BoundingBox merge(const BoundingBox& other) const;

    float dx() const;
    float dy() const;
    float dz() const;
    float volume() const;
};
