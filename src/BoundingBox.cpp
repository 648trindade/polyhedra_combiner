#include "BoundingBox.h"

void BoundingBox::update(const Vertex& vertex)
{
    max.x = std::max(vertex.x, max.x);
    max.y = std::max(vertex.y, max.y);
    max.z = std::max(vertex.z, max.z);
    min.x = std::min(vertex.x, min.x);
    min.y = std::min(vertex.y, min.y);
    min.z = std::min(vertex.z, min.z);
}

bool BoundingBox::overlap(const BoundingBox& other) const
{
    return !(numeric::is_great(other.min.x, this->max.x)
        || numeric::is_less(other.max.x, this->min.x)
        || numeric::is_great(other.min.y, this->max.y)
        || numeric::is_less(other.max.y, this->min.y)
        || numeric::is_great(other.min.z, this->max.z)
        || numeric::is_less(other.max.z, this->min.z));
}

float BoundingBox::dx() const
{
    return this->max.x - this->min.x;
}

float BoundingBox::dy() const
{
    return this->max.y - this->min.y;
}

float BoundingBox::dz() const
{
    return this->max.z - this->min.z;
}

float BoundingBox::volume() const
{
    return this->dx() * this->dy() * this->dz();
}

bool BoundingBox::overlap(const Vertex& v) const
{
    return numeric::is_less_or_close(this->min.x, v.x)
        && numeric::is_less_or_close(v.x, this->max.x)
        && numeric::is_less_or_close(this->min.y, v.y)
        && numeric::is_less_or_close(v.y, this->max.y)
        && numeric::is_less_or_close(this->min.z, v.z)
        && numeric::is_less_or_close(v.z, this->max.z); 
}

BoundingBox BoundingBox::merge(const BoundingBox& other) const
{
    BoundingBox merge = *this;
    merge.update(other.max);
    merge.update(other.min);
    return merge;
}
