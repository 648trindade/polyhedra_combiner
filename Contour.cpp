#include "Contour.h"

void Contour::add_edge(const EdgeInfo& edge)
{
    size_t position = this->edges.size();
    this->edges.push_back(edge);
    const_cast<Edge*>(edge.first)->contours.insert({this, position});
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

bool Contour::overlap(const Vertex& point, const Vertex& normal) const
{
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
        if (direction.cross(point_to_edge).dot(normal) > 0)
        {
            return false;
        }
    }
    return true;
}
