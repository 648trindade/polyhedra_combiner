#include "BoundingBox.h"

void BoundingBox::update(Vertex vertex)
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
    return !(
        (other.min.x > this->max.x) || (other.max.x < this->min.x) || (other.min.y > this->max.y)
        || (other.max.y < this->min.y) || (other.min.z > this->max.z)
        || (other.max.z < this->min.z));
}
