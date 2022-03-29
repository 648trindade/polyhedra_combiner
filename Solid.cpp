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

EdgeInfo Solid::add_edge(const Vertex* begin, const Vertex* end)
{
    auto match = this->find_edge(begin, end);
    if (match.first)
    {
        return match;
    }
    auto [iterator, inserted] = this->edges.insert(Edge{begin, end});
    return std::make_pair(&(*iterator), true);
}

EdgeInfo Solid::find_edge(const Vertex* begin, const Vertex* end)
{
    Edge forward{begin, end};
    auto match = this->edges.find(forward);
    if (match != this->edges.end())
    {
        return std::make_pair(&(*match), true);
    }
    Edge backward{end, begin};
    match = this->edges.find(backward);
    if (match != this->edges.end())
    {
        return std::make_pair(&(*match), false);
    }
    return std::make_pair(nullptr, true);
}
