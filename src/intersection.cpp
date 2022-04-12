#include "intersection.h"
#include <iostream>
#include <tuple>

void face_intersect(Face& home_face, Face& near_face);

void intersect(Solid& home_solid, Solid& near_solid)
{
    const BoundingBox home_box = home_solid.bounding_box();
    const BoundingBox near_box = home_solid.bounding_box();
    if (!home_box.overlap(near_box))
    {
        return;
    }

    for (auto home_it = home_solid.faces.begin(); home_it != home_solid.faces.end(); home_it++)
    {
        Face& home_face = *home_it;
        const BoundingBox home_face_box = home_face.bounding_box();
        for (Face& near_face : near_solid.faces)
        {
            if (!home_face_box.overlap(near_face.bounding_box()))
            {
                continue;
            }
            if (home_face.are_coplanar(near_face))
            {
                continue; // TODO polygon clipper
            }
            else
            {
                face_intersect(home_face, near_face);
            }
        }
    }
}

void face_intersect(
    Face& home_face, Face& near_face, std::vector<Face>& home_faces, std::vector<Face>& near_faces)
{
//    using Intersection = std::tuple<bool, bool, int, Vertex>;
//    std::vector<Intersection> intersections;

    auto find_intersections = [/*&intersections*/](Face& face_a, Face& face_b, bool home) -> bool
    {
        for (int i = 0; i < face_a.get_number_of_edges(); i++)
        {
            const Edge edge_a = face_a.get_edge(i);
            auto [edge_intersect, point] = face_b.intersect(edge_a);
            if (edge_intersect && face_b.intersect(point))
            {
                return true;
//                if (!intersections.empty())
//                {
//                    // keep just the lastest id
//                    auto& [p_home, p_is_vertex, p_id, p_point] = intersections.back();
//                    if (p_home == home && p_point == point)
//                    {
//                        p_is_vertex = true;
//                        continue;
//                    }
//                }
//                intersections.emplace_back(std::make_tuple(true, false, i, point));
            }
        }
        return false;
    };
    bool has_intersection = find_intersections(home_face, near_face, true);
    if (!has_intersection && !find_intersections(near_face, home_face, false))
    {
        return;
    }

    auto [u, v] = home_face.intersect(near_face);
    auto split_face = [/*&intersections,*/ line_vector = u, line_point = v](
                          bool home, Face& face, std::vector<Face>& faces)
    {
//        size_t count = std::count_if(intersections.begin(), intersections.end(), [home](auto& in) {
//            return std::get<0>(in) == home;
//        });
//        Vertex entry, exit;
//        int entry_id, exit_id;
//        if (count == 0)
//        {
//
//        }
        Face new_face = face.split(line_vector, line_point);
    };
    split_face(true, home_face, home_faces);
    split_face(false, near_face, near_faces)
}

void intersect_solids(std::vector<Solid>& solids)
{
    for (size_t i = 0; i < solids.size(); i++)
    {
        for (size_t j = i + 1; j < solids.size(); j++)
        {
            intersect(solids[i], solids[j]);
        }
    }
}