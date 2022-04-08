#include "Edge.h"

bool Edge::operator<(const Edge& other) const
{
    return (*this->start < *other.start)
        || ((*this->start == *other.start) && (*this->end < *other.end));
}

bool Edge::operator==(const Edge& other) const
{
    return (*this->start == *other.start) && (*this->end == *other.end);
}

Vertex Edge::get_direction() const
{
    return *this->end - *this->start;
}

Vertex Edge::get_normal() const
{
    return this->get_direction().normalize();
}

float Edge::get_length() const
{
    return this->get_direction().norm();
}

bool Edge::intersect(const Vertex& point) const
{
    const Vertex start_to_point = point - *this->start;
    if (Vertex::is_close(start_to_point.norm(), 0))
    {
        return true;
    }

    const float dot_product = this->get_normal().dot(start_to_point.normalize());
    if (dot_product < 0)
    {
        return false;
    }
    else if (!Vertex::is_close(dot_product, 1))
    {
        return false;
    }
    else
    {
        const float distance = start_to_point.norm();
        const float length = this->get_length();
        return (distance < length) || Vertex::is_close(distance, length);
    }
}
