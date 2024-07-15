// Oleg Kotov

#include "vector3i.h"
#include "vector.h"

#include <math.h>

Vector3i::Vector3i()
{
	x = 0;
	y = 0;
	z = 0;
}

Vector3i::Vector3i( int x, int y, int z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3i::Vector3i( const Vector& v )
{
	x = v.x;
	y = v.y;
	z = v.z;
}

Vector3i& Vector3i::operator=( const Vector& other )
{
	x = other.x;
	y = other.y;
	z = other.z;

	return *this;
}

Vector3i Vector3i::operator-( const Vector3i& v ) const
{
	Vector3i result;

	result.x = x - v.x;
	result.y = y - v.y;
	result.z = z - v.z;

	return result;
}

Vector3i Vector3i::operator+( const Vector& v ) const
{
	Vector3i result;

	result.x = x + v.x;
	result.y = y + v.y;
	result.z = z + v.z;

	return result;

}

