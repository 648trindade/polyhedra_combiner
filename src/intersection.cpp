#include "intersection.h"
#include <tuple>

std::tuple<bool, Face, Face> face_intersect(Face& home_face, Face& near_face);

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
        const size_t n_near_faces = near_solid.faces.size();
        for (size_t i = 0; i < n_near_faces; i++)
        {
            Face& near_face = near_solid.faces[i];
            if (!home_face_box.overlap(near_face.bounding_box()))
            {
                continue;
            }
            if (home_face.is_coplanar(near_face))
            {
                if (home_face.intersect_coplanar(near_face))
                {
                    continue; // TODO
                }
            }
            else
            {
                auto [intersect, new_home_face, new_near_face] =
                    std::move(face_intersect(home_face, near_face));
                if (intersect)
                {
                    if (new_home_face.get_number_of_vertices() > 0)
                    {
                        home_solid.faces.emplace_back(std::move(new_home_face));
                    }
                    if (new_near_face.get_number_of_vertices() > 0)
                    {
                        near_solid.faces.emplace_back(std::move(new_near_face));
                    }
                }
            }
        }
    }
}

std::tuple<bool, Face, Face> face_intersect(Face& home_face, Face& near_face)
{
    bool has_intersection = home_face.edges_intersect(near_face);
    if (!has_intersection && !near_face.edges_intersect(home_face))
    {
        return std::make_tuple(false, Face {}, Face {});
    }

    auto [line_vector, line_point] = home_face.get_intersection_line(near_face);
    return std::make_tuple(
        true, home_face.split(line_point, line_vector), near_face.split(line_point, line_vector));
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