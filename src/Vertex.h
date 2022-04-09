#pragma once

#include <ostream>
#include <vector>

/*!
 * A vertex specifies a point in space given by three coordinates. Its data structure contains a
 * triple of floating-point coordinates and a tolerance indicating their accuracy. In addition,
 * there is a list of all edges to which the vertex is connected.
 */
class Vertex
{
public:
    float x, y, z;

    Vertex operator+(const Vertex& other) const;
    Vertex& operator+=(const Vertex& other);
    Vertex operator-(const Vertex& other) const;
    Vertex& operator-=(const Vertex& other);
    Vertex operator*(const Vertex& other) const;
    Vertex& operator*=(const Vertex& other);
    Vertex operator/(const Vertex& other) const;
    Vertex& operator/=(const Vertex& other);

    bool operator<(const Vertex& other) const;
    bool operator==(const Vertex& other) const;
    Vertex operator-() const;

    Vertex operator+(float value) const;
    Vertex& operator+=(float value);
    Vertex operator-(float value) const;
    Vertex& operator-=(float value);
    Vertex operator*(float value) const;
    Vertex& operator*=(float value);
    Vertex operator/(float value) const;
    Vertex& operator/=(float value);

    float dot() const;
    float dot(const Vertex& other) const;
    Vertex cross(const Vertex& other) const;
    float norm() const;
    Vertex normalize() const;
    Vertex rotate(const float rotation_matrix[3][3]) const;

    float abs_dot(const Vertex& other) const;
    Vertex cross(const Vertex& other, const Vertex& another) const;

    friend std::ostream& operator<<(std::ostream& stream, const Vertex& v);
};

std::ostream& operator<< (std::ostream& stream, const Vertex& v);