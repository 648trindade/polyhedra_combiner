#include "Solid.h"

Face& Solid::add_face()
{
    this->faces.push_back(Face{});
    return this->faces.back();
}

const Vertex* Solid::add_vertex(const Vertex& vertex)
{
    auto [iterator, inserted] = this->vertexes.insert(vertex);
    return &(*iterator);
}

const Edge* Solid::add_edge(const Vertex* begin, const Vertex* end)
{
    auto [iterator, inserted] = this->edges.insert(Edge{begin, end});
    return &(*iterator);
}
