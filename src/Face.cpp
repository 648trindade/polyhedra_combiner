#include "Face.h"

#include "numeric.h"

#include <cmath>
#include <set>

Face::Face(std::vector<Vertex>& vertices)
    : vertices(std::move(vertices))
{
    this->compute_plane_equation();
}

Face::Face(Face&& other)
    : vertices(std::move(other.vertices))
    , normal(other.normal)
    , distance(other.distance)
{
}

void Face::add_vertex(const Vertex& vertex)
{
    this->vertices.push_back(vertex);
}

void Face::compute_plane_equation()
{
    const Vertex& a = this->vertices[0];
    const Vertex& b = this->vertices[1];
    const Vertex& c = this->vertices[2];
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

size_t Face::get_number_of_vertices() const
{
    return this->vertices.size();
}

size_t Face::get_number_of_edges() const
{
    return (this->vertices.size() > 1) ? this->vertices.size() : 0;
}

Edge Face::get_edge(size_t id) const
{
    const size_t next_id = (id + 1) % this->vertices.size();
    return Edge { this->vertices[id], this->vertices[next_id] };
}

BoundingBox Face::bounding_box() const
{
    BoundingBox box {};
    for (auto& vertex : this->vertices)
    {
        box.update(vertex);
    }
    return box;
}

std::pair<bool, Vertex> Face::intersect(const Edge& edge) const
{
    const float start_distance = this->normal.dot(edge.start);
    const float end_distance = this->normal.dot(edge.end);

    if (numeric::is_close(end_distance, this->distance)
        && numeric::is_close(end_distance, start_distance))
    {
        return std::make_pair(true, edge.start);
    }
    else if (
        (start_distance <= this->distance) && (this->distance <= end_distance)
        || (end_distance <= this->distance) && (this->distance <= start_distance))
    {
        const float fraction = (this->distance - start_distance) / (end_distance - start_distance);
        Vertex point = edge.start + (edge.get_direction() * fraction);
        return std::make_pair(true, point);
    }
    else
    {
        return std::make_pair(false, Vertex {});
    }
}

bool Face::intersect(const Vertex& point) const
{
    for (int i = 0; i < this->get_number_of_edges(); i++)
    {
        const Edge edge = this->get_edge(i);
        const Vertex direction = edge.get_normal();
        const Vertex point_to_edge = (point - edge.start).normalize();
        const Vertex triangle_normal = direction.cross(point_to_edge);
        const bool at_left = numeric::is_great(triangle_normal.dot(this->normal), 0);
        if (at_left)
        {
            return false;
        }
    }
    return true;
}

Vertex Face::get_center() const
{
    Vertex center {};
    for (auto& vertex : this->vertices)
    {
        center += vertex;
    }
    return center / this->get_number_of_vertices();
}

bool Face::is_convex() const
{
    return this->intersect(this->get_center());
}
