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

std::pair<bool, Vertex> Edge::intersect(const Edge& other) const
{
    auto [intersect, point] = other.intersect(this->start, this->get_normal());
    const Vertex start_to_point = point - this->start;
    if (!intersect)
    {
        return std::make_pair(false, Vertex {});
    }
    else if (numeric::is_less(start_to_point.normalize().dot(this->get_normal()), 0))
    {
        return std::make_pair(false, Vertex {}); // point before start
    }
    else if (numeric::is_great_or_close(start_to_point.norm(), this->get_length()))
    {
        return std::make_pair(false, Vertex {}); // point at or after end
    }
    else
    {
        return std::make_pair(intersect, point);
    }
}

std::pair<bool, Vertex> Edge::intersect(Vertex line_point, Vertex line_vector) const
{
    const Vertex normal = this->get_normal();
    const Vertex line_to_start = this->start - line_point;
    const Vertex line_to_start_normal = line_to_start.normalize();

    float line_start_angle = line_vector.dot(line_to_start_normal);
    if (numeric::is_close(normal.abs_dot(line_vector), 1)) // are parallel
    {
        if (numeric::is_close(std::fabs(line_start_angle), 1))
        {
            return std::make_pair(true, this->start); // same line
        }
        else
        {
            return std::make_pair(false, Vertex {}); // different lines
        }
    }
    else if (numeric::is_less(line_start_angle, 0))
    {
        line_vector = -line_vector; // make vector point to edge
        line_start_angle = line_vector.dot(line_to_start_normal);
    }

    const Vertex plane_normal = line_point.cross(this->start, this->end).normalize();
    const float plane_distance = plane_normal.dot(line_point);
    if (!numeric::is_close(plane_normal.dot(line_point + line_vector), plane_distance))
    {
        return std::make_pair(false, Vertex {}); // different planes
    }

    const Vertex line_perp = line_vector.cross(line_to_start_normal).cross(line_vector).normalize();
    const float proj_size_start = line_perp.dot(line_to_start);
    const float proj_size_end = line_perp.dot(this->end - line_point);
    if (std::signbit(proj_size_start) == std::signbit(proj_size_end))
    {
        return std::make_pair(false, Vertex {}); // both points aside from line
    }

    // we make a triangle with sides a (line -> start) b (start -> point) c (line -> point)
    const float A = std::acos(line_start_angle);
    const float B = std::acos(normal.dot(-line_to_start_normal));
    const float C = M_PI - A - B;
    const float c = line_to_start.norm();
    const float length = c * std::sin(A) / std::sin(C);
    const Vertex point = this->start + normal * length;
    return std::make_pair(true, point);
}
