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
    : name(other.name)
    , faces(std::move(other.faces))
{
}

Face& Solid::add_face()
{
    this->faces.push_back(Face{});
    return this->faces.back();
}

Face& Solid::add_face(std::vector<Vertex>&& vertices)
{
    this->faces.emplace_back(vertices);
    return this->faces.back();
}

Face& Solid::add_face(Face&& face)
{
    this->faces.push_back(std::move(face));
    return this->faces.back();
}

BoundingBox Solid::bounding_box() const
{
    BoundingBox box {};
    for (auto& face : this->faces)
    {
        box = box.merge(face.bounding_box());
    }
    return box;
}

bool Solid::is_inside(const Face& other_face) const
{
    const BoundingBox box = this->bounding_box();
    const Vertex center { (box.max + box.min) * 0.5 };
    for (const Face& face : this->faces)
    {
        const Vertex face_center = face.get_center();
        const Vertex center_to_face = face_center - center;
        for (const Vertex& point : other_face.vertices)
        {
            const Vertex face_to_point = point - face_center;
            const float dot = face_to_point.dot(center_to_face);
            if (numeric::is_great(dot, 1))
            {
                return false;
            }
        }
    }
    return true;
}

void Solid::to_OBJ(const std::string filename) const
{
    std::ofstream file(filename);
    std::set<Vertex> vertices_set;

    auto find_vertex_index = [&](const Vertex& v) -> size_t
    {
        auto it = vertices_set.find(v);
        return std::distance(vertices_set.begin(), it) + 1;
    };

    file << "o " << this->name << std::endl;

    for (auto& face : this->faces)
    {
        vertices_set.insert(face.vertices.begin(), face.vertices.end());
    }
    size_t center_offset = vertices_set.size();
    for (auto& vertex : vertices_set)
    {
        file << "v " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
    }

    for (auto& face : this->faces)
    {
        for (auto& vertex : face.vertices)
        {
            file << "f";
            for (auto& v : face.vertices)
            {
                file << " " << find_vertex_index(v);
            }
            file << std::endl;
//            if (face.get_number_of_vertices() == 3)
//            {
//                file << "f " << find_vertex_index(face.vertices[0]) << " "
//                     << find_vertex_index(face.vertices[1]) << " "
//                     << find_vertex_index(face.vertices[2]) << std::endl;
//            }
//            else if (face.get_number_of_vertices() == 4)
//            {
//                file << "f " << find_vertex_index(face.vertices[0]) << " "
//                     << find_vertex_index(face.vertices[1]) << " "
//                     << find_vertex_index(face.vertices[2]) << std::endl;
//                file << "f " << find_vertex_index(face.vertices[0]) << " "
//                     << find_vertex_index(face.vertices[2]) << " "
//                     << find_vertex_index(face.vertices[3]) << std::endl;
//            }
//            else
//            {
//                Vertex center = face.get_center();
//                file << "v " << center.x << " " << center.y << " " << center.z << std::endl;
//                for (int i = 0; i < face.get_number_of_edges(); i++)
//                {
//                    const Edge edge = face.get_edge(i);
//                    file << "f " << find_vertex_index(edge.start) << " "
//                         << find_vertex_index(edge.end) << " " << center_offset << std::endl;
//                }
//                center_offset++;
//            }
        }
    }

    file.close();
}

void Solid::add_external_face(Face* face)
{
    this->external_faces.emplace_back(face);
}
