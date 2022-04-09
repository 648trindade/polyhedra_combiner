#include "intersection.h"
#include <iostream>
#include <tuple>

void face_intersect(Face& face1, Face& face2);

template <typename Object1, typename Object2>
bool intersect(Object1& object1, Object2& object2)
{
    if (!object1.bounding_box().overlap(object2.bounding_box()))
    {
        return false;
    }

    if constexpr (std::is_same<Object1, Face>::value && std::is_same<Object2, Face>::value)
    {
        face_intersect(object1, object2);
    }
    else if constexpr (std::is_same<Object1, Solid>::value)
    {
        for (Face& face : object1.faces)
        {
            intersect(face, object2);
        }
    }
    else
    {
        for (Face& face : object2.faces)
        {
            intersect(object1, face);
        }
    }
    return true;
}

void remove_intersect(std::vector<Solid>& solids)
{
    for (size_t i = 0; i < solids.size(); i++)
    {
        for (size_t j = i + 1; j < solids.size(); j++)
        {
            intersect(solids[i], solids[j]);
        }
    }
}

void face_intersect(Face& face1, Face& face2)
{
    using Intersection = std::tuple<int, Vertex>;
    std::vector<Intersection> intersections;

    for (int i = 0; i < face1.get_number_of_edges(); i++)
    {
        const Edge home_edge = face1.get_edge(i);
        auto [edge_intersect, point] = face2.intersect(home_edge);
        if (edge_intersect && face2.intersect(point))
        {
            intersections.emplace_back(std::make_tuple(i, point));
        }
    }
    for (auto& [i, point] : intersections)
    {
        Edge edge = face1.get_edge(i);
        std::cout << "Edge " << edge.start << " " << edge.end << std::endl;
        std::cout << "Point " << point << std::endl;
        std::cout << "Plane " << face2.normal << " " << face2.distance << std::endl << std::endl;
    }
}