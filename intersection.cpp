#include "intersection.h"

//template <typename Object>
//void remove_intersect(Object& object);

void face_intersect(Face& face1, Face& face2)
{
    return;
}

template <typename Object1, typename Object2>
void intersect(Object1& object1, Object2& object2)
{
    if (!object1.bounding_box().overlap(object2.bounding_box()))
    {
        return;
    }

    if constexpr (std::is_same<Object1, Face>::value && std::is_same<Object2, Face>::value)
    {
        face_intersect(object1, object2);
    }
    else if constexpr (std::is_same<Object1, Solid>::value)
    {
        for (Face& face : object1.faces)
        {
//            remove_intersect(face);
            intersect(face, object2);
        }
    }
    else
    {
        for (Face& face : object2.faces)
        {
//            remove_intersect(face);
            intersect(object1, face);
        }
    }
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

//template<>
//void remove_intersect<Solid>(Solid& solid)
//{
//
//}
//
//template<>
//void remove_intersect<Face>(Face& face)
//{
//
//}