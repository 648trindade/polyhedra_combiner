#pragma once

#include "Edge.h"

#include <list>

class Face;

/*!
 * A contour represents a single closed planar polygonal curve. Self-intersecting contours are not
 * allowed. A contour may assume one of two orientations; a component edge may be used in either
 * direction. Each contour structure is a singly linked list of pointers to edges. Each list
 * element also shows whether the edge is used in the defined or reversed direction. The list
 * elements are circularly linked, facilitating the splitting and merging of contours.
 */
class Contour
{
public:
    Face* face;
    std::list<EdgeInfo> edges;

    void add_edge(const EdgeInfo& edge_info);
    void add_edge(const Edge* previous, const EdgeInfo& edge);

    void merge(const Contour& other);
    bool overlap(const Vertex& point) const;
    Vertex get_center() const;
    bool is_convex() const;
};
