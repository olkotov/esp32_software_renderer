// Oleg Kotov

#include "vector4.h"
#include "vector.h"

Vector4::Vector4()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    w = 0.0f;
}

Vector4::Vector4( float x, float y, float z, float w )
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Vector4::Vector4( const Vector& v, float w )
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
    this->w = w;
}

Vector4 Vector4::operator*( const float f ) const
{
    Vector4 result;

    result.x = x * f;
    result.y = y * f;
    result.z = z * f;
    result.w = w * f;

    return result;
}

Vector4 Vector4::operator*=( const float f )
{
    *this = *this * f;
    return *this;
}

