#include "Edge.h"

#include "numeric.h"

std::pair<bool, Vertex> intersect_lines(Vertex C, Vertex e, Vertex D, Vertex f)
{
    const Vertex g = D - C;
    const Vertex fxg = f.cross(g);
    const Vertex fxe = f.cross(e);
    const float h = fxg.norm();
    const float k = fxe.norm();
    if (numeric::is_close(h, 0) || numeric::is_close(k, 0))
    {
        return std::make_pair(false, Vertex {});
    }
    else if (numeric::is_close(fxg.normalize().dot(fxe.normalize()), 1))
    {
        const Vertex point = C + (e * (h / k));
        return std::make_pair(true, point);
    }
    else
    {
        const Vertex point = C - (e * (h / k));
        return std::make_pair(true, point);
    }
}

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
    if (this->start == point)
    {
        return true;
    }
    else if (this->end == point)
    {
        return false;
    }

    const Vertex start_to_point = point - this->start;
    const float dot_product = this->get_normal().dot((start_to_point).normalize());
    if (numeric::is_close(dot_product, 1))
    {
        return numeric::is_less(start_to_point.norm(), this->get_length());
    }
    else
    {
        return false;
    }
}

std::pair<bool, Vertex> Edge::intersect(const Edge& other) const
{
    auto [intersect, point] = other.intersect(this->start, this->get_normal());
    if (intersect && this->intersect(point))
    {
        return std::make_pair(intersect, point);
    }
    else
    {
        return std::make_pair(false, Vertex {});
    }
}

std::pair<bool, Vertex> intersect_lines(Vertex v_a, Vertex u_a, Vertex v_b, Vertex u_b);

std::pair<bool, Vertex> Edge::intersect(Vertex line_point, Vertex line_vector) const
{
    auto [intersect, point] =
        intersect_lines(this->start, this->get_normal(), line_point, line_vector);
    if (intersect && this->intersect(point))
    {
        return std::make_pair(intersect, point);
    }
    else
    {
        return std::make_pair(false, Vertex {});
    }
}
