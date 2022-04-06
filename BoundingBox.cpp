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
    return !((other.min.x > this->max.x)
        || (other.max.x < this->min.x)
        || (other.min.y > this->max.y)
        || (other.max.y < this->min.y)
        || (other.min.z > this->max.z)
        || (other.max.z < this->min.z));
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
    return (this->min.x <= v.x && v.x <= this->max.x)
        && (this->min.y <= v.y && v.y <= this->max.y)
        && (this->min.z <= v.z && v.z <= this->max.z);
}

BoundingBox BoundingBox::merge(const BoundingBox& other) const
{
    BoundingBox merge = *this;
    merge.update(other.max);
    merge.update(other.min);
    return merge;
}
