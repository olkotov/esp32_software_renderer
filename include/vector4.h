// Oleg Kotov

#pragma once

class Vector;

class Vector4
{
public:

	Vector4();
	Vector4( float x, float y, float z, float w );
	Vector4( const Vector& v, float w );


public:

	Vector4 operator*( const float f ) const;
	Vector4 operator*=( const float f );

public:

	float x;
	float y;
	float z;
	float w;
};

