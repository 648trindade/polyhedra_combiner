#pragma once

#include "BoundingBox.h"
#include "Edge.h"

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
    std::vector<Vertex> vertices;
    Vertex normal;
    float distance;

    Face() = default;
    Face(std::vector<Vertex>& vertices);
    Face(Face& other);
    Face(Face&& other);

    void add_vertex(const Vertex& vertex);

    void compute_plane_equation();
    std::pair<Vertex, Vertex> intersect(const Face& other) const;

    size_t get_number_of_vertices() const;
    size_t get_number_of_edges() const;
    Edge get_edge(size_t id) const;

    BoundingBox bounding_box() const;

    std::pair<bool, Vertex> intersect(const Edge& edge) const;
    std::pair<bool, Vertex> intersect_coplanar(
        const Vertex& line_vector, const Vertex& line_point) const;
    bool intersect(const Vertex& point) const;
    Vertex get_center() const;
    bool is_convex() const;

    Face split(int first_edge, Vertex first_point, int second_edge, Vertex second_point);
    Face split(const Vertex& line_vector, const Vertex& line_point);
};
