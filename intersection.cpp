#include "intersection.h"

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
    using Intersection = std::tuple<EdgeInfo, Vertex, Contour*>;
    std::vector<Intersection> intersections;

    for (const Contour& home_contour : face1.contours)
    {
        for (const EdgeInfo& edge_info : home_contour.edges)
        {
            auto [edge_intersect, point] = face2.edge_plane_intersect(*edge_info.first);
            if (edge_intersect)
            {
                for (Contour& near_contour : face2.contours)
                {
                    if (near_contour.overlap(point, face2.normal))
                    {
                        intersections.emplace_back(edge_info, point, &near_contour);
                    }
                }
            }
        }
    }
}