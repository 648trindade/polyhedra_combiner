#include "Face.h"

#include <iostream>

#include <cfloat>
#include <cmath>
#include <set>

Contour& Face::add_contour()
{
    this->contours.push_back(Contour{});
    return this->contours.back();
}

void Face::compute_plane_equation()
{
    Contour& first_contour = this->contours.front();
    auto iterator = first_contour.edges.begin();
    const Vertex& a = *this->get_vertex(0);
    const Vertex& b = *this->get_vertex(1);
    const Vertex& c = *this->get_vertex(2);
    this->normal = (b - a).cross(c - a).normalize();
    this->distance = this->normal.dot(a);
}

std::pair<Vertex, Vertex> Face::intersect(const Face& other) const
{
    const Vertex& n1 = this->normal;
    const Vertex& n2 = other.normal;
    const Vertex u = n1.cross(n2).normalize();
    // (u x n2 * d1) + (n1 x u * d2) / det
    const Vertex v = ((u.cross(n2) * this->distance) + (n1.cross(u) * other.distance)) / u.dot();
    return std::make_pair(u, v);
}

template <typename Op>
std::set<const Vertex*> iter_over_vertexes(const Face* face, Op op)
{
    std::set<const Vertex*> vertexes;
    bool inserted;
    for (auto& contour : face->contours)
    {
        for (auto& edge_info : contour.edges)
        {
            if (vertexes.insert(edge_info.first->start).second)
            {
                op(edge_info.first->start, vertexes.size());
            }
            if (vertexes.insert(edge_info.first->end).second)
            {
                op(edge_info.first->end, vertexes.size());
            }
        }
    }
    return vertexes;
}

size_t Face::get_number_of_vertexes() const
{
    std::set<const Vertex*> vertexes { iter_over_vertexes(this, [](auto a, auto b) {}) };
    return vertexes.size();
}

const Vertex* Face::get_vertex(size_t id)
{
    const Vertex* result = nullptr;
    iter_over_vertexes(this, [&result, id](const Vertex* vertex, size_t size) -> void {
        if (size == id + 1)
        {
            result = vertex;
        }
    });
    return result;
}

BoundingBox Face::bounding_box() const
{
    BoundingBox box{};
    iter_over_vertexes(
        this, [&box](const Vertex* vertex, size_t size) -> void { box.update(*vertex); });
    return box;
}

std::pair<bool, Vertex> Face::edge_plane_intersect(const Edge& edge) const
{
    const float start_distance = this->normal.dot(*edge.start);
    const float end_distance = this->normal.dot(*edge.end);
    if (std::fabs(end_distance - start_distance) < std::numeric_limits<float>::epsilon())
    {
        return std::make_pair(true, *edge.start);
    }
    else if ((start_distance <= this->distance) && (this->distance <= end_distance)
        || (end_distance <= this->distance) && (this->distance <= start_distance))
    {
        const float fraction = (this->distance - start_distance) / (end_distance - start_distance);
        Vertex point = *edge.start + (edge.get_direction() * fraction);
        return std::make_pair(true, point);
    }
    else
    {
        return std::make_pair(false, Vertex{});
    }
}
