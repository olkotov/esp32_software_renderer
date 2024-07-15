// Oleg Kotov

#pragma once

class Vector3i;

class Vector
{
public:

	Vector();
	Vector( float x, float y, float z );

	Vector( const Vector3i& v );

public:

	Vector operator-();

	Vector operator+( const Vector& vec ) const;
	Vector operator+=( const Vector& vec );

	Vector operator-( const Vector& vec ) const;
	Vector operator-=( const Vector& vec );

	Vector operator*( const float scale ) const;
	Vector operator*=( const float scale );

public:

	static float dotProduct( const Vector& a, const Vector& b );
	static Vector crossProduct( const Vector& a, const Vector& b );

	Vector normalize();

public:

	float x;
	float y;
	float z;
};

