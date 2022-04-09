#include "Edge.h"

#include "numeric.h"

bool Edge::operator==(const Edge& other) const
{
    return (this->start == other.start) && (this->end == other.end);
}

Vertex Edge::get_direction() const
{
    return this->end - this->start;
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
    const Vertex start_to_point = point - this->start;
    const float distance = start_to_point.norm();
    if (numeric::is_close(distance, 0))
    {
        return true;
    }

    const float dot_product = this->get_normal().dot(start_to_point.normalize());
    if (dot_product < 0)
    {
        return false;
    }
    else if (!numeric::is_close(dot_product, 1))
    {
        return false;
    }
    else
    {
        const float length = this->get_length();
        return numeric::is_less_or_close(distance, length);
    }
}
