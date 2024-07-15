// Oleg Kotov

#pragma once

class Matrix;
class Vector;

class Matrix3
{
public:

	Matrix3();
    Matrix3( const Matrix& mat );

public:

	float* operator[]( int i );
	const float* operator[]( int i ) const;

public:

    // Returns a transposed matrix.
    Matrix3 transposed() const;

	// Returns an inverse of the matrix.
    Matrix3 inverse() const;

    Vector transformPoint( const Vector& point ) const;

public:

    static const Matrix3 Identity;

private:

	float m_data[3][3];
};

