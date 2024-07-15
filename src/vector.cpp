// Oleg Kotov

#include "vector.h"
#include "vector3i.h"

#include <math.h>

Vector::Vector()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vector::Vector( float x, float y, float z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector::Vector( const Vector3i& v )
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
}

Vector Vector::operator-()
{
    return Vector( -x, -y, -z );
}

Vector Vector::operator+( const Vector& v ) const
{
	Vector result;

	result.x = x + v.x;
	result.y = y + v.y;
	result.z = z + v.z;

	return result;
}

Vector Vector::operator+=( const Vector& other )
{
    *this = *this + other;
    return *this;
}

Vector Vector::operator-( const Vector& v ) const
{
	Vector result;

	result.x = x - v.x;
	result.y = y - v.y;
	result.z = z - v.z;

	return result;
}

Vector Vector::operator-=( const Vector& other )
{
    *this = *this - other;
    return *this;
}

Vector Vector::operator*( const float scale ) const
{
    return Vector( x * scale, y * scale, z * scale );
}

Vector Vector::operator*=( const float scale )
{
    *this = *this * scale;
    return *this;
}

float Vector::dotProduct( const Vector& a, const Vector& b )
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector Vector::crossProduct( const Vector& a, const Vector& b )
{
	Vector result;

    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;

    return result;
}

Vector Vector::normalize()
{
	float lengthSq = ( x * x + y * y + z * z );

    if ( lengthSq > 0.0f )
    {
        float invLength = 1.0f / sqrtf( lengthSq );
        
        x *= invLength;
        y *= invLength;
        z *= invLength;
    }

	return *this;
}

