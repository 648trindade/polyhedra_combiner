#pragma once

#include "Face.h"

#include <set>

class Solid
{
public:
    Solid();
    Solid(const std::string& name);
    Solid(Solid&& other);

    std::string name;
    std::vector<Face> faces;
    std::set<Vertex> vertexes;
    std::set<Edge> edges;

    Face& add_face();
    EdgeInfo find_edge(const Vertex* begin, const Vertex* end);
    EdgeInfo add_edge(const Vertex* begin, const Vertex* end);

    const Vertex* add_vertex(const Vertex& vertex);
    BoundingBox bounding_box() const;

    void to_OBJ(const std::string filename) const;
};
