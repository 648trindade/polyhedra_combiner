#include "Solid.h"

#include <fstream>

Solid::Solid()
    : name("Solid")
{
}

Solid::Solid(const std::string& name)
    : name(name)
{
}

Solid::Solid(Solid&& other)
{
    this->name = other.name;
    this->vertexes = std::move(other.vertexes);
    this->edges = std::move(other.edges);
    this->faces = std::move(other.faces);
}

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

BoundingBox Solid::bounding_box() const
{
    BoundingBox box{};
    for (auto& vertex : this->vertexes)
    {
        box.update(vertex);
    }
    return box;
}

void Solid::to_OBJ(const std::string filename) const
{
    std::ofstream file(filename);
    size_t center_offset = this->vertexes.size() + 1;

    auto find_vertex_index = [&](const Vertex* v) -> size_t
    {
        auto it = this->vertexes.find(*v);
        return std::distance(this->vertexes.begin(), it) + 1;
    };

    file << "o " << this->name << std::endl;

    for (auto& vertex : this->vertexes)
    {
        file << "v " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
    }
    for (auto& face : this->faces)
    {
        for (auto& contour : face.contours)
        {
            auto edge_info = contour.edges.begin();
            if (contour.edges.size() == 3)
            {
                file << "f " << find_vertex_index(edge_info->first->start) << " "
                     << find_vertex_index(std::next(edge_info, 1)->first->start) << " "
                     << find_vertex_index(std::next(edge_info, 2)->first->start) << std::endl;
            }
            else if (contour.edges.size() == 4)
            {
                file << "f " << find_vertex_index(edge_info->first->start) << " "
                     << find_vertex_index(std::next(edge_info, 1)->first->start) << " "
                     << find_vertex_index(std::next(edge_info, 2)->first->start) << std::endl;
                file << "f " << find_vertex_index(edge_info->first->start) << " "
                     << find_vertex_index(std::next(edge_info, 2)->first->start) << " "
                     << find_vertex_index(std::next(edge_info, 3)->first->start) << std::endl;
            }
            else
            {
                Vertex center = contour.get_center();
                file << "v " << center.x << " " << center.y << " " << center.z << std::endl;
                for (auto& [edge, forward] : contour.edges)
                {
                    file << "f " << find_vertex_index(edge->start) << " "
                         << find_vertex_index(edge->end) << " " << center_offset << std::endl;
                }
                center_offset++;
            }
        }
    }

    file.close();
}
