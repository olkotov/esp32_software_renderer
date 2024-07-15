// Oleg Kotov

#include "matrix.h"
#include "vector.h"
#include "vector4.h"

#include <math.h>

const Matrix Matrix::Identity;

Matrix::Matrix()
{
	m_data[0][0] = 1.0f; m_data[0][1] = 0.0f; m_data[0][2] = 0.0f; m_data[0][3] = 0.0f;
	m_data[1][0] = 0.0f; m_data[1][1] = 1.0f; m_data[1][2] = 0.0f; m_data[1][3] = 0.0f;
	m_data[2][0] = 0.0f; m_data[2][1] = 0.0f; m_data[2][2] = 1.0f; m_data[2][3] = 0.0f;
	m_data[3][0] = 0.0f; m_data[3][1] = 0.0f; m_data[3][2] = 0.0f; m_data[3][3] = 1.0f;
}

float* Matrix::operator[]( int i )
{
	return m_data[i];
}

const float* Matrix::operator[]( int i ) const
{
	return m_data[i];
}

Matrix& Matrix::operator=( const Matrix& m )
{
	m_data[0][0] = m[0][0]; m_data[0][1] = m[0][1]; m_data[0][2] = m[0][2]; m_data[0][3] = m[0][3];
	m_data[1][0] = m[1][0]; m_data[1][1] = m[1][1]; m_data[1][2] = m[1][2]; m_data[1][3] = m[1][3];
	m_data[2][0] = m[2][0]; m_data[2][1] = m[2][1]; m_data[2][2] = m[2][2]; m_data[2][3] = m[2][3];
	m_data[3][0] = m[3][0]; m_data[3][1] = m[3][1]; m_data[3][2] = m[3][2]; m_data[3][3] = m[3][3];

	return *this;
}

Matrix Matrix::operator*( const Matrix& m ) const
{
	Matrix result;

	result[0][0] = m_data[0][0] * m[0][0] + m_data[0][1] * m[1][0] + m_data[0][2] * m[2][0] + m_data[0][3] * m[3][0];
	result[0][1] = m_data[0][0] * m[0][1] + m_data[0][1] * m[1][1] + m_data[0][2] * m[2][1] + m_data[0][3] * m[3][1];
	result[0][2] = m_data[0][0] * m[0][2] + m_data[0][1] * m[1][2] + m_data[0][2] * m[2][2] + m_data[0][3] * m[3][2];
	result[0][3] = m_data[0][0] * m[0][3] + m_data[0][1] * m[1][3] + m_data[0][2] * m[2][3] + m_data[0][3] * m[3][3];

	result[1][0] = m_data[1][0] * m[0][0] + m_data[1][1] * m[1][0] + m_data[1][2] * m[2][0] + m_data[1][3] * m[3][0];
	result[1][1] = m_data[1][0] * m[0][1] + m_data[1][1] * m[1][1] + m_data[1][2] * m[2][1] + m_data[1][3] * m[3][1];
	result[1][2] = m_data[1][0] * m[0][2] + m_data[1][1] * m[1][2] + m_data[1][2] * m[2][2] + m_data[1][3] * m[3][2];
	result[1][3] = m_data[1][0] * m[0][3] + m_data[1][1] * m[1][3] + m_data[1][2] * m[2][3] + m_data[1][3] * m[3][3];

	result[2][0] = m_data[2][0] * m[0][0] + m_data[2][1] * m[1][0] + m_data[2][2] * m[2][0] + m_data[2][3] * m[3][0];
	result[2][1] = m_data[2][0] * m[0][1] + m_data[2][1] * m[1][1] + m_data[2][2] * m[2][1] + m_data[2][3] * m[3][1];
	result[2][2] = m_data[2][0] * m[0][2] + m_data[2][1] * m[1][2] + m_data[2][2] * m[2][2] + m_data[2][3] * m[3][2];
	result[2][3] = m_data[2][0] * m[0][3] + m_data[2][1] * m[1][3] + m_data[2][2] * m[2][3] + m_data[2][3] * m[3][3];

	result[3][0] = m_data[3][0] * m[0][0] + m_data[3][1] * m[1][0] + m_data[3][2] * m[2][0] + m_data[3][3] * m[3][0];
	result[3][1] = m_data[3][0] * m[0][1] + m_data[3][1] * m[1][1] + m_data[3][2] * m[2][1] + m_data[3][3] * m[3][1];
	result[3][2] = m_data[3][0] * m[0][2] + m_data[3][1] * m[1][2] + m_data[3][2] * m[2][2] + m_data[3][3] * m[3][2];
	result[3][3] = m_data[3][0] * m[0][3] + m_data[3][1] * m[1][3] + m_data[3][2] * m[2][3] + m_data[3][3] * m[3][3];

	return result;
}

Vector4 Matrix::operator*( const Vector4& v ) const
{
	Vector4 result;

	result.x = m_data[0][0] * v.x + m_data[0][1] * v.y + m_data[0][2] * v.z + m_data[0][3] * v.w;
	result.y = m_data[1][0] * v.x + m_data[1][1] * v.y + m_data[1][2] * v.z + m_data[1][3] * v.w;
	result.z = m_data[2][0] * v.x + m_data[2][1] * v.y + m_data[2][2] * v.z + m_data[2][3] * v.w;
	result.w = m_data[3][0] * v.x + m_data[3][1] * v.y + m_data[3][2] * v.z + m_data[3][3] * v.w;

	return result;
}

Matrix Matrix::transposed() const
{
    Matrix result;
    
    result[0][0] = m_data[0][0];
    result[0][1] = m_data[1][0];
    result[0][2] = m_data[2][0];
    result[0][3] = m_data[3][0];
    
    result[1][0] = m_data[0][1];
    result[1][1] = m_data[1][1];
    result[1][2] = m_data[2][1];
    result[1][3] = m_data[3][1];
    
    result[2][0] = m_data[0][2];
    result[2][1] = m_data[1][2];
    result[2][2] = m_data[2][2];
    result[2][3] = m_data[3][2];
    
    result[3][0] = m_data[0][3];
    result[3][1] = m_data[1][3];
    result[3][2] = m_data[2][3];
    result[3][3] = m_data[3][3];
    
    return result;
}

Matrix Matrix::inverse() const
{
    Matrix result;
    
    float t00 = m_data[0][0] * m_data[1][1] - m_data[0][1] * m_data[1][0];
    float t01 = m_data[0][0] * m_data[1][2] - m_data[0][2] * m_data[1][0];
    float t02 = m_data[0][0] * m_data[1][3] - m_data[0][3] * m_data[1][0];
    float t03 = m_data[0][1] * m_data[1][2] - m_data[0][2] * m_data[1][1];
    float t04 = m_data[0][1] * m_data[1][3] - m_data[0][3] * m_data[1][1];
    float t05 = m_data[0][2] * m_data[1][3] - m_data[0][3] * m_data[1][2];
    float t06 = m_data[2][0] * m_data[3][1] - m_data[2][1] * m_data[3][0];
    float t07 = m_data[2][0] * m_data[3][2] - m_data[2][2] * m_data[3][0];
    float t08 = m_data[2][0] * m_data[3][3] - m_data[2][3] * m_data[3][0];
    float t09 = m_data[2][1] * m_data[3][2] - m_data[2][2] * m_data[3][1];
    float t10 = m_data[2][1] * m_data[3][3] - m_data[2][3] * m_data[3][1];
    float t11 = m_data[2][2] * m_data[3][3] - m_data[2][3] * m_data[3][2];

    float det = ( t00 * t11 - t01 * t10 + t02 * t09 + t03 * t08 - t04 * t07 + t05 * t06 );
    
	// The determinant must not be zero.

    if ( det == 0.0f ) return Matrix::Identity;

    float invDet = 1.0f / det;

    result[0][0] = (  m_data[1][1] * t11 - m_data[1][2] * t10 + m_data[1][3] * t09 ) * invDet;
    result[0][1] = ( -m_data[0][1] * t11 + m_data[0][2] * t10 - m_data[0][3] * t09 ) * invDet;
    result[0][2] = (  m_data[3][1] * t05 - m_data[3][2] * t04 + m_data[3][3] * t03 ) * invDet;
    result[0][3] = ( -m_data[2][1] * t05 + m_data[2][2] * t04 - m_data[2][3] * t03 ) * invDet;
        
    result[1][0] = ( -m_data[1][0] * t11 + m_data[1][2] * t08 - m_data[1][3] * t07 ) * invDet;
    result[1][1] = (  m_data[0][0] * t11 - m_data[0][2] * t08 + m_data[0][3] * t07 ) * invDet;
    result[1][2] = ( -m_data[3][0] * t05 + m_data[3][2] * t02 - m_data[3][3] * t01 ) * invDet;
    result[1][3] = (  m_data[2][0] * t05 - m_data[2][2] * t02 + m_data[2][3] * t01 ) * invDet;
        
    result[2][0] = (  m_data[1][0] * t10 - m_data[1][1] * t08 + m_data[1][3] * t06 ) * invDet;
    result[2][1] = ( -m_data[0][0] * t10 + m_data[0][1] * t08 - m_data[0][3] * t06 ) * invDet;
    result[2][2] = (  m_data[3][0] * t04 - m_data[3][1] * t02 + m_data[3][3] * t00 ) * invDet;
    result[2][3] = ( -m_data[2][0] * t04 + m_data[2][1] * t02 - m_data[2][3] * t00 ) * invDet;
        
    result[3][0] = ( -m_data[1][0] * t09 + m_data[1][1] * t07 - m_data[1][2] * t06 ) * invDet;
    result[3][1] = (  m_data[0][0] * t09 - m_data[0][1] * t07 + m_data[0][2] * t06 ) * invDet;
    result[3][2] = ( -m_data[3][0] * t03 + m_data[3][1] * t01 - m_data[3][2] * t00 ) * invDet;
    result[3][3] = (  m_data[2][0] * t03 - m_data[2][1] * t01 + m_data[2][2] * t00 ) * invDet;
    
    return result;
}

Vector Matrix::transformPoint( const Vector& point ) const
{
    Vector result;

    result.x = m_data[0][0] * point.x + m_data[0][1] * point.y + m_data[0][2] * point.z + m_data[0][3];
    result.y = m_data[1][0] * point.x + m_data[1][1] * point.y + m_data[1][2] * point.z + m_data[1][3];
    result.z = m_data[2][0] * point.x + m_data[2][1] * point.y + m_data[2][2] * point.z + m_data[2][3];

    return result;
}

Matrix Matrix::setIdentity()
{
	m_data[0][0] = 1.0f; m_data[0][1] = 0.0f; m_data[0][2] = 0.0f; m_data[0][3] = 0.0f;
	m_data[1][0] = 0.0f; m_data[1][1] = 1.0f; m_data[1][2] = 0.0f; m_data[1][3] = 0.0f;
	m_data[2][0] = 0.0f; m_data[2][1] = 0.0f; m_data[2][2] = 1.0f; m_data[2][3] = 0.0f;
	m_data[3][0] = 0.0f; m_data[3][1] = 0.0f; m_data[3][2] = 0.0f; m_data[3][3] = 1.0f;

	return *this;
}

Matrix Matrix::setTranslation( float x, float y, float z )
{
	m_data[0][0] = 1.0f; m_data[0][1] = 0.0f; m_data[0][2] = 0.0f; m_data[0][3] = x;
	m_data[1][0] = 0.0f; m_data[1][1] = 1.0f; m_data[1][2] = 0.0f; m_data[1][3] = y;
	m_data[2][0] = 0.0f; m_data[2][1] = 0.0f; m_data[2][2] = 1.0f; m_data[2][3] = z;
	m_data[3][0] = 0.0f; m_data[3][1] = 0.0f; m_data[3][2] = 0.0f; m_data[3][3] = 1.0f;

	return *this;
}

Matrix Matrix::setFromAxisAngle( const Vector& axis, float angle )
{
	float cos = cosf( angle );
	float sin = sinf( angle );
	float t = 1.0f - cos;
	float tx = t * axis.x;
	float ty = t * axis.y;

	m_data[0][0] = tx * axis.x + cos;
	m_data[1][0] = tx * axis.y + sin * axis.z;
	m_data[2][0] = tx * axis.z - sin * axis.y;

	m_data[0][1] = tx * axis.y - sin * axis.z;
	m_data[1][1] = ty * axis.y + cos;
	m_data[2][1] = ty * axis.z + sin * axis.x;

	m_data[0][2] = tx * axis.z + sin * axis.y;
	m_data[1][2] = ty * axis.z - axis.x * sin;
	m_data[2][2] = t * axis.z * axis.z + cos;

	m_data[3][3] = 1.0f;

	return *this;
}

Matrix Matrix::setScaling( float x, float y, float z )
{
	m_data[0][0] = x;    m_data[0][1] = 0.0f; m_data[0][2] = 0.0f; m_data[0][3] = 0.0f;
	m_data[1][0] = 0.0f; m_data[1][1] = y;    m_data[1][2] = 0.0f; m_data[1][3] = 0.0f;
	m_data[2][0] = 0.0f; m_data[2][1] = 0.0f; m_data[2][2] = z;    m_data[2][3] = 0.0f;
	m_data[3][0] = 0.0f; m_data[3][1] = 0.0f; m_data[3][2] = 0.0f; m_data[3][3] = 1.0f;

	return *this;
}

Matrix Matrix::setLookAt( const Vector& position, const Vector& target, const Vector& up )
{
	Vector f = Vector( target - position ).normalize();
	Vector s = Vector::crossProduct( up, f ).normalize();
	Vector u = Vector::crossProduct( f, s );

	m_data[0][0] = s.x;
	m_data[1][0] = u.x;
	m_data[2][0] = f.x;

	m_data[0][1] = s.y;
	m_data[1][1] = u.y;
	m_data[2][1] = f.y;

	m_data[0][2] = s.z;
	m_data[1][2] = u.z;
	m_data[2][2] = f.z;

	m_data[0][3] = -Vector::dotProduct( s, position );
	m_data[1][3] = -Vector::dotProduct( u, position );
	m_data[2][3] = -Vector::dotProduct( f, position );

	m_data[3][3] = 1.0f;

	return *this;
}

Matrix Matrix::setPerspective( float fov, float aspect, float near, float far )
{
	float tan = tanf( fov / 2.0f );

	m_data[0][0] = 1.0f / ( aspect * tan );
	m_data[1][1] = 1.0f / ( tan );
	m_data[2][2] = ( far + near ) / ( far - near );
	m_data[3][2] = 1.0f;
	m_data[2][3] = -( 2.0f * far * near ) / ( far - near );

	return *this;
}

// Matrix Matrix::makePerspective( float vfov, float aspect, float near, float far )
// {
// 	Matrix mat;

// 	// focalLength
// 	// cot(fovY/2) - the same
// 	float yScale = 1.0f / tanf( vfov * 0.5f );
//     float xScale = yScale / aspect;

//     float m22 = -( far + near ) / ( far - near );
//     float m23 = near * far * 2.0f / ( near - far );

//     mat[0][0] = xScale;
//     mat[1][1] = yScale;

//     mat[2][2] = m22;
//     mat[2][3] = m23;

//     mat[3][2] = -1.0f;
//     mat[3][3] = 0.0f; // !

// 	return mat;
// }

Matrix Matrix::makePerspectiveReversedRH( float vfov, float aspect, float near, float far )
{
	Matrix mat;

    float yScale = 1.0f / tanf( vfov * 0.5f );
    float xScale = yScale / aspect;

    float m22 = -( far / ( near - far ) ) - 1.0f;
    float m23 = -( near * far / ( near - far ) );

    mat[0][0] = xScale;
    mat[1][1] = yScale;

    mat[2][2] = m22;
    mat[2][3] = m23;

    mat[3][2] = -1.0f;
    mat[3][3] = 0.0f; // !

	return mat;
}

Matrix Matrix::makePerspectiveRH( float vfov, float aspect, float near, float far )
{
	Matrix mat;

    float yScale = 1.0f / tanf( vfov * 0.5f );
    float xScale = yScale / aspect;

    float m22 = -( far + near ) / ( far - near );
    float m23 = near * far * 2.0f / ( near - far );

    mat[0][0] = xScale;
    mat[1][1] = yScale;

    mat[2][2] = m22;
    mat[2][3] = m23;

    mat[3][2] = -1.0f;
    mat[3][3] = 0.0f; // !

	return mat;
}

Matrix Matrix::makeLookAtRH( const Vector& position, const Vector& target, const Vector& worldUp )
{
	Matrix mat;

    Vector zaxis = ( position - target ).normalize();
    Vector xaxis = Vector::crossProduct( worldUp, zaxis ).normalize();
    Vector yaxis = Vector::crossProduct( zaxis, xaxis );

    mat[0][0] = xaxis.x;
    mat[1][0] = yaxis.x;
    mat[2][0] = zaxis.x;
    
    mat[0][1] = xaxis.y;
    mat[1][1] = yaxis.y;
    mat[2][1] = zaxis.y;
    
    mat[0][2] = xaxis.z;
    mat[1][2] = yaxis.z;
    mat[2][2] = zaxis.z;
    
    mat[0][3] = Vector::dotProduct( xaxis, position );
    mat[1][3] = Vector::dotProduct( yaxis, position );
    mat[2][3] = Vector::dotProduct( zaxis, position );

	return mat;
}

