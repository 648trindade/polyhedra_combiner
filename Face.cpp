#include "Face.h"

Contour& Face::add_contour()
{
    this->contours.push_back(Contour{});
    return this->contours.back();
}

void Face::compute_plane_equation()
{
    Contour& first_contour = this->contours[0];
    auto iterator = first_contour.edges.begin();
    const Vertex& a = *((*iterator++).edge->start);
    const Vertex& b = *((*iterator++).edge->start);
    const Vertex& c = *((*iterator++).edge->start);
    this->normal = (b - a).cross(c - a).normalize();
    this->distance = this->normal.dot(a);
}
