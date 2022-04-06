#pragma once

#include "BoundingBox.h"
#include "Contour.h"

#include <iterator>

/*!
 * A face specifies by one or more contours a set of finite two-dimensional patches in the same
 * plane. A normal and a distance from the origin define the plane in which the patches lie. Each
 * contour divides the face's plane into two areas: a finite area “inside” the contour and an
 * infinite area "outside" the contour. The cross product of the face normal with the
 * direction of a contour edge points from the edge into the region enclosed by the contour. A
 * contour enclosing its outside is called a hole.
 * Each contour must bound an open planar set so that local topological determinations are possible.
 * This restriction ensures that a contour does not contain zero-width "spikes," although zero-width
 * "notches" are allowed.
 * The representation of a face consists of a list of contours and four real numbers describing its
 * plane equation. A tolerance indicates the accuracy of the plane equation’s coefficients. If
 * appropriate, each face also points to the solid to which it belongs.
 */
class Face
{
public:
    std::vector<Contour> contours;
    Vertex normal;
    float distance;

    Contour& add_contour();
    void compute_plane_equation();
    std::pair<Vertex, Vertex> intersect(const Face& other) const;

    size_t get_number_of_vertexes() const;
    const Vertex* get_vertex(size_t id);

    BoundingBox bounding_box() const;

    std::pair<bool, Vertex> edge_plane_intersect(const Edge& edge) const;
};
