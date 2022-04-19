#include "intersection.h"
#include <tuple>

void intersect(Solid& home_solid, Solid& near_solid)
{
    const BoundingBox home_box = home_solid.bounding_box();
    const BoundingBox near_box = near_solid.bounding_box();
    if (!home_box.overlap(near_box))
    {
        return;
    }

    std::vector<size_t> last_near_face(home_solid.faces.size(), 0);
    for (size_t home_face_id = 0; home_face_id < home_solid.faces.size(); home_face_id++)
    {
        const BoundingBox home_face_box = home_solid.faces[home_face_id].bounding_box();
        const size_t first_near_face = last_near_face[home_face_id];
        const size_t n_near_faces = near_solid.faces.size();
        for (size_t near_face_id = first_near_face; near_face_id < n_near_faces; near_face_id++)
        {
            Face& home_face = home_solid.faces[home_face_id];
            Face& near_face = near_solid.faces[near_face_id];
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
                        last_near_face.push_back(near_face_id);
                        home_solid.to_OBJ("/tmp/a_" + std::to_string(home_face_id) + "_" + std::to_string(near_face_id) + ".obj");
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
        true,
        std::move(home_face.split(line_point, line_vector)),
        std::move(near_face.split(line_point, line_vector)));
}

void draw_new_surfaces(Solid& home_solid, Solid& near_solid)
{
    const BoundingBox home_box = home_solid.bounding_box();
    const BoundingBox near_box = near_solid.bounding_box();
    if (!home_box.overlap(near_box))
    {
        return;
    }

    // disable old external surfaces
    for (Face& near_face : near_solid.faces)
    {
        const BoundingBox face_box = near_face.bounding_box();
        if (near_face.enabled && face_box.overlap(home_box) && !home_solid.is_inside(near_face))
        {
            near_face.enabled = false;
        }
    }

    // filter new external surfaces
    for (Face& home_face : home_solid.faces)
    {
        const BoundingBox face_box = home_face.bounding_box();
        if (home_face.enabled && face_box.overlap(near_box) && near_solid.is_inside(home_face))
        {
            near_solid.add_external_face(&home_face);
        }
    }
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
    for (size_t i = 0; i < solids.size(); i++)
    {
        for (size_t j = i + 1; j < solids.size(); j++)
        {
            draw_new_surfaces(solids[i], solids[j]);
        }
    }
}