#pragma once

#include "Vertex.h"

#include <map>

class Contour;

/*!
 * An edge joins a pair of vertices defining its endpoints. Each edge structure has pointers to its
 * starting vertex and to its ending vertex. In addition, a list indicates in which contour(s) of
 * which face(s) the edge is used and the corresponding location within those contour.
 */
class Edge
{
public:
    const Vertex *start, *end;
    std::map<Contour*, size_t> contours;

    Vertex get_direction() const;
    Vertex get_normal() const;
    float get_length() const;

    bool operator<(const Edge& other) const;
    bool operator==(const Edge& other) const;
};

typedef std::pair<const Edge*, bool> EdgeInfo;