#include "Vertex.h"

#include "numeric.h"

#include <cmath>

Vertex Vertex::operator+(const Vertex& other) const
{
    return Vertex { this->x + other.x, this->y + other.y, this->z + other.z };
}

Vertex& Vertex::operator+=(const Vertex& other)
{
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    return *this;
}

Vertex Vertex::operator-(const Vertex& other) const
{
    return Vertex { this->x - other.x, this->y - other.y, this->z - other.z };
}

Vertex& Vertex::operator-=(const Vertex& other)
{
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    return *this;
}

Vertex Vertex::operator*(const Vertex& other) const
{
    return Vertex { this->x * other.x, this->y * other.y, this->z * other.z };
}

Vertex& Vertex::operator*=(const Vertex& other)
{
    this->x *= other.x;
    this->y *= other.y;
    this->z *= other.z;
    return *this;
}

Vertex Vertex::operator/(const Vertex& other) const
{
    return Vertex { this->x / other.x, this->y / other.y, this->z / other.z };
}

Vertex& Vertex::operator/=(const Vertex& other)
{
    this->x /= other.x;
    this->y /= other.y;
    this->z /= other.z;
    return *this;
}

bool Vertex::operator<(const Vertex& other) const
{
    return (numeric::is_less(this->x, other.x))
        || (numeric::is_close(this->x, other.x) && numeric::is_less(this->y, other.y))
        || (numeric::is_close(this->x, other.x) && numeric::is_close(this->y, other.y)
            && numeric::is_less(this->z, other.z));
}

bool Vertex::operator==(const Vertex& other) const
{
    return numeric::is_close(this->x, other.x) && numeric::is_close(this->y, other.y)
        && numeric::is_close(this->z, other.z);
}

Vertex Vertex::operator-() const
{
    return Vertex { -this->x, -this->y, -this->z };
}

Vertex Vertex::operator+(const float value) const
{
    return Vertex { this->x + value, this->y + value, this->z + value };
}

Vertex& Vertex::operator+=(const float value)
{
    this->x += value;
    this->y += value;
    this->z += value;
    return *this;
}

Vertex Vertex::operator-(const float value) const
{
    return Vertex { this->x - value, this->y - value, this->z - value };
}

Vertex& Vertex::operator-=(const float value)
{
    this->x -= value;
    this->y -= value;
    this->z -= value;
    return *this;
}

Vertex Vertex::operator*(const float value) const
{
    return Vertex { this->x * value, this->y * value, this->z * value };
}

Vertex& Vertex::operator*=(const float value)
{
    this->x *= value;
    this->y *= value;
    this->z *= value;
    return *this;
}

Vertex Vertex::operator/(const float value) const
{
    return Vertex { this->x / value, this->y / value, this->z / value };
}

Vertex& Vertex::operator/=(const float value)
{
    this->x /= value;
    this->y /= value;
    this->z /= value;
    return *this;
}

float Vertex::dot() const
{
    return this->dot(*this);
}

float Vertex::dot(const Vertex& other) const
{
    return this->x * other.x + this->y * other.y + this->z * other.z;
}

float Vertex::abs_dot(const Vertex& other) const
{
    return std::fabs(this->dot(other));
}

Vertex Vertex::cross(const Vertex& other) const
{
    return Vertex {
        this->y * other.z - this->z * other.y,
        this->z * other.x - this->x * other.z,
        this->x * other.y - this->y * other.x,
    };
}

float Vertex::norm() const
{
    return std::sqrt(this->dot());
}

Vertex Vertex::normalize() const
{
    return *this / this->norm();
}

Vertex Vertex::cross(const Vertex& other, const Vertex& another) const
{
    return (other - *this).cross(another - *this);
}

Vertex Vertex::rotate(const float (*rotation_matrix)[3]) const
{
    return Vertex {
        this->dot(Vertex { rotation_matrix[0][0], rotation_matrix[0][1], rotation_matrix[0][2] }),
        this->dot(Vertex { rotation_matrix[1][0], rotation_matrix[1][1], rotation_matrix[1][2] }),
        this->dot(Vertex { rotation_matrix[2][0], rotation_matrix[2][1], rotation_matrix[2][2] }),
    };
}

std::ostream& operator<<(std::ostream& stream, const Vertex& v)
{
    stream << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return stream;
}
