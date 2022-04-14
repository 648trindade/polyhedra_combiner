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

    std::set<int> discarded_faces;
    std::vector<Face*> external_faces;

    Face& add_face();
    Face& add_face(std::vector<Vertex>& vertices);
    Face& add_face(Face&& face);

    BoundingBox bounding_box() const;

    bool is_inside(const Face& face) const;

    void to_OBJ(const std::string filename) const;
};
