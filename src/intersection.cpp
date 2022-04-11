#include "intersection.h"
#include <iostream>
#include <tuple>

void face_intersect(Face& home_face, Face& near_face);

template <typename Object1, typename Object2>
bool intersect(Object1& home_object, Object2& near_object)
{
    if (!home_object.bounding_box().overlap(near_object.bounding_box()))
    {
        return false;
    }

    if constexpr (std::is_same<Object1, Solid>::value)
    {
        for (auto face = home_object.faces.begin(); face != home_object.faces.end(); face++)
        {
            intersect(*face, near_object);
        }
    }
    else if constexpr (std::is_same<Object2, Solid>::value)
    {
        for (auto face = near_object.faces.begin(); face != near_object.faces.end(); face++)
        {
            intersect(home_object, *face);
        }
    }
    else
    {
        intersect(home_object, near_object);
    }
    return true;
}

template<>
bool intersect(Face& home_face, Face& near_face)
{
    using Intersection = std::tuple<int, Vertex>;
    std::vector<Intersection> intersections;

    for (int i = 0; i < home_face.get_number_of_edges(); i++)
    {
        const Edge home_edge = home_face.get_edge(i);
        auto [edge_intersect, point] = near_face.intersect(home_edge);
        if (edge_intersect && near_face.intersect(point))
        {
            intersections.emplace_back(std::make_tuple(i, point));
        }
    }
    for (auto& [i, point] : intersections)
    {
        Edge edge = home_face.get_edge(i);
        std::cout << "Edge " << edge.start << " " << edge.end << std::endl;
        std::cout << "Point " << point << std::endl;
        std::cout << "Plane " << near_face.normal << " " << near_face.distance << std::endl << std::endl;
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