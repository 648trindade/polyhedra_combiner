#include "Contour.h"

#include "Face.h"

void Contour::add_edge(const EdgeInfo& edge_info)
{
    const size_t position = this->edges.size();
    this->edges.push_back(edge_info);
    Edge* edge = const_cast<Edge*>(edge_info.first);
    edge->contours.insert({this, position});
}

void Contour::add_edge(const Edge* previous, const EdgeInfo& edge)
{
    size_t position = 0;
    auto it = this->edges.begin();
    for (; it->first != previous && it != this->edges.end(); it++, position++);
    this->edges.insert(it, edge);

    Edge* new_edge = const_cast<Edge*>(edge.first);
    new_edge->contours[this] = ++position;
    for(++it; it != this->edges.end(); it++)
    {
        Edge* edge = const_cast<Edge*>(it->first);
        edge->contours[this]++;
    }
}

void Contour::merge(const Contour& other)
{

}

bool Contour::overlap(const Vertex& point) const
{
    const Vertex& face_normal = this->face->normal;
    for (const auto& [edge, reverse] : this->edges)
    {
        Vertex direction, point_to_edge;
        if (!reverse)
        {
            direction = edge->get_normal();
            point_to_edge = (point - *edge->start).normalize();
        }
        else
        {
            direction = -edge->get_normal();
            point_to_edge = (point - *edge->end).normalize();
        }
        if (direction.cross(point_to_edge).dot(face_normal) > 0)
        {
            return false;
        }
    }
    return true;
}

Vertex Contour::get_center() const
{
    Vertex center {};
    for (auto& [edge, forward] : this->edges)
    {
        center += *edge->start;
    }
    return center / this->edges.size();
}

bool Contour::is_convex() const
{
    return this->overlap(this->get_center());
}
