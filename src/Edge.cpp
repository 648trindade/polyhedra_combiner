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

std::pair<bool, Vertex> Edge::intersect(const Edge& other, bool are_coplanar) const
{
    const Vertex normal = this->get_normal();
    const Vertex other_normal = other.get_normal();
    const Vertex to_start = other.start - this->start;
    const Vertex to_end = other.end - this->start;

    auto normal_cross = [&normal](const Vertex& a) -> Vertex
    {
        return normal.cross(a).normalize();
    };

    if (!are_coplanar
        && !numeric::is_close(normal_cross(to_start).abs_dot(normal_cross(to_end)), 1))
    {
        return { false, Vertex {} };
    }
    if (numeric::is_close(normal.abs_dot(other_normal), 1))
    {
        const float length = this->get_length();
    }

    return std::make_pair<bool, Vertex>(0, {});
}
