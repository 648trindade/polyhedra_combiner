#pragma once

#include "Vertex.h"

/*!
 * An edge joins a pair of vertices defining its endpoints. Each edge structure has pointers to its
 * starting vertex and to its ending vertex. In addition, a list indicates in which contour(s) of
 * which face(s) the edge is used and the corresponding location within those contour.
 */
class Edge
{
public:
    const Vertex start, end;

    Vertex get_direction() const;
    Vertex get_normal() const;
    float get_length() const;

    bool intersect(const Vertex& point) const;
    std::pair<bool, Vertex> intersect(const Edge& edge) const;
    std::pair<bool, Vertex> intersect(Vertex line_point, Vertex line_vector) const;

    bool operator==(const Edge& other) const;
};