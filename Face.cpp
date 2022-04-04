#include "Face.h"

#include <iostream>

#include <cmath>
#include <cfloat>

Contour& Face::add_contour()
{
    this->contours.push_back(Contour{});
    return this->contours.back();
}

void Face::compute_plane_equation()
{
    Contour& first_contour = this->contours[0];
    auto iterator = first_contour.edges.begin();
    const Vertex& a = *((*iterator++).first->start);
    const Vertex& b = *((*iterator++).first->start);
    const Vertex& c = *((*iterator++).first->start);
    this->normal = (b - a).cross(c - a).normalize();
    this->distance = this->normal.dot(a);
}

std::pair<Vertex, Vertex> Face::intersect(const Face& other) const
{
    const Vertex x = *this->contours[0].edges.front().first->start;

    Vertex n1 = this->normal;
    Vertex n2 = other.normal;
    Vertex u = n1.cross(n2).normalize();

    if (std::fabs(n1.dot(n2)) > FLT_EPSILON)
    {
        std::cerr << "May be wrong" << std::endl;
        n1 = n2.cross(u).normalize();
    }
    Vertex d_ { n1.dot(x), n2.dot(x), u.dot(x) };
    // v = Q^T . d'
    Vertex v { Vertex { n1.x, n2.x, u.x }.dot(d_),
               Vertex { n1.y, n2.y, u.y }.dot(d_),
               Vertex { n1.z, n2.z, u.z }.dot(d_) };
    return std::make_pair(u, v);
}