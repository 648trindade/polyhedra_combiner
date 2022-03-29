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
