// Oleg Kotov

#pragma once

class Vector;

class Vector3i
{
public:

	Vector3i();
	Vector3i( int x, int y, int z );
	Vector3i( const Vector& v );

public:

	Vector3i& operator=( const Vector& other );

	Vector3i operator-( const Vector3i& v ) const;
	Vector3i operator+( const Vector& v ) const;

public:

	int x;
	int y;
	int z;
};

