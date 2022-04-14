#include "Face.h"

#include "numeric.h"

#include <cmath>
#include <set>
#include <string>
#include <tuple>

Face::Face(std::vector<Vertex>& vertices)
    : vertices(std::move(vertices))
{
    this->compute_plane_equation();
}

Face::Face(Face& other)
    : vertices(other.vertices)
    , normal(other.normal)
    , distance(other.distance)
{
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
    this->normal = a.cross(b, c).normalize();
    this->distance = this->normal.dot(a);
}

std::pair<Vertex, Vertex> Face::get_intersection_line(const Face& other) const
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

bool Face::intersect_coplanar(const Face& other) const
{
    if (!this->is_coplanar(other))
    {
        return false;
    }
    for (int i = 0; i < this->get_number_of_edges(); i++)
    {
        const Edge edge = this->get_edge(i);
        for (int j = 0; j < other.get_number_of_edges(); j++)
        {
            const Edge other_edge = other.get_edge(j);
            if (edge.intersect(other_edge).first)
            {
                return true;
            }
        }
    }
    return false;
}

bool Face::intersect(const Vertex& point) const
{
    for (int i = 0; i < this->get_number_of_edges(); i++)
    {
        const Edge edge = this->get_edge(i);
        if (point == edge.start || point == edge.end) // point on edge extremes
        {
            return true;
        }

        const Vertex direction = edge.get_normal();
        const Vertex point_to_edge = point - edge.start;
        const float dot = direction.dot(point_to_edge.normalize());
        if (numeric::is_close(dot, 1)) // point is aligned with edge
        {
            return numeric::is_less_or_close(point_to_edge.norm(), edge.get_length());
        }
        else if (numeric::is_close(dot, -1)) // point aligned with edge in opposite direction
        {
            return false;
        }

        const Vertex pair_normal = direction.cross(point_to_edge).normalize();
        const bool at_right = numeric::is_close(this->normal.dot(pair_normal), 1);
        if (!at_right) // if at left of edge, then it is outside of face
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

Face Face::split(const Vertex& line_point, const Vertex& line_vector)
{
    std::vector<Vertex> remaining;
    std::vector<Vertex> removed;
    int n_intersections = 0;
    Vertex points[2] { {}, {} };
    int edge_id[2] { -1, -1 };
    bool cut_on_vertex[2] { false, false };
    for (int i = 0; i < this->get_number_of_edges(); i++)
    {
        Edge edge = this->get_edge(i);
        auto [intersect, point] = edge.intersect(line_point, line_vector);
        if (intersect)
        {
            edge_id[n_intersections] = i;
            points[n_intersections] = point;
            cut_on_vertex[n_intersections] = (point == this->vertices[i]);
            n_intersections++;
        }
    }
    if (n_intersections == 0)
    {
        return Face {};
    }
    else if (n_intersections != 2)
    {
        throw std::runtime_error(
            "Unexpected number of n_intersections: " + std::to_string(n_intersections));
    }
    const size_t next_id = (edge_id[0] + 1) % this->get_number_of_vertices();
    const auto vertex_begin = this->vertices.begin();
    const auto vertex_end = this->vertices.end();
    const auto vertex_0_end = vertex_begin + edge_id[0] + 1;
    const auto vertex_1_end = vertex_begin + edge_id[1] + 1;
    if (cut_on_vertex[0] && cut_on_vertex[1] && (next_id == edge_id[1]))
    {
        return Face {};
    }
    remaining.assign(vertex_begin, vertex_0_end);
    if (cut_on_vertex[0])
    {
        removed.push_back(this->vertices[edge_id[0]]);
    }
    else
    {
        remaining.push_back(points[0]);
        removed.push_back(points[0]);
    }
    removed.insert(removed.end(), vertex_0_end, vertex_1_end);
    if (cut_on_vertex[1])
    {
        remaining.push_back(this->vertices[edge_id[1]]);
    }
    else
    {
        remaining.push_back(points[1]);
        removed.push_back(points[1]);
    }
    remaining.insert(remaining.end(), vertex_1_end, vertex_end);
    this->vertices.swap(remaining);
    return { removed };
}

bool Face::is_coplanar(const Face& other) const
{
    if (this->normal == other.normal)
    {
        return numeric::is_close(this->distance, other.distance);
    }
    else if (this->normal == -other.normal)
    {
        return numeric::is_close(this->distance, -other.distance);
    }
    return false;
}

bool Face::edges_intersect(const Face& other) const
{
    for (int i = 0; i < this->get_number_of_edges(); i++)
    {
        const Edge edge = this->get_edge(i);
        auto [edge_intersect, point] = other.intersect(edge);
        if (edge_intersect && other.intersect(point))
        {
            return true;
        }
    }
    return false;
}
