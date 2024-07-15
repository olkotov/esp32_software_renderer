// Oleg Kotov

#pragma once

class Vector;
class Vector4;

class Matrix
{
public:

	Matrix();

public:

	float* operator[]( int i );
	const float* operator[]( int i ) const;

	Matrix& operator=( const Matrix& m );
	
	Matrix operator*( const Matrix& m ) const;
	Vector4 operator*( const Vector4& v ) const;

public:

    Matrix transposed() const;
    Matrix inverse() const;

	Vector transformPoint( const Vector& point ) const;

	Matrix setIdentity();
	Matrix setTranslation( float x, float y, float z );
	Matrix setFromAxisAngle( const Vector& axis, float angle );
	Matrix setScaling(float x, float y, float z);
	Matrix setLookAt( const Vector& position, const Vector& target, const Vector& up );
	Matrix setPerspective( float fov, float aspect, float near, float far );

	static Matrix makePerspectiveReversedRH( float vfov, float aspect, float near, float far );
	static Matrix makePerspectiveRH( float vfov, float aspect, float near, float far );
	static Matrix makeLookAtRH( const Vector& position, const Vector& target, const Vector& worldUp );

public:

	static const Matrix Identity;

private:

	float m_data[4][4];
};

