// Oleg Kotov

#include "matrix3.h"
#include "matrix.h"
#include "vector.h"

const Matrix3 Matrix3::Identity;

Matrix3::Matrix3()
{
	m_data[0][0] = 0.0f; m_data[0][1] = 0.0f; m_data[0][2] = 0.0f;
	m_data[1][0] = 0.0f; m_data[1][1] = 0.0f; m_data[1][2] = 0.0f;
	m_data[2][0] = 0.0f; m_data[2][1] = 0.0f; m_data[2][2] = 0.0f;
}

Matrix3::Matrix3( const Matrix& m )
{
    m_data[0][0] = m[0][0]; m_data[0][1] = m[0][1]; m_data[0][2] = m[0][0];
	m_data[1][0] = m[1][0]; m_data[1][1] = m[1][1]; m_data[1][2] = m[0][0];
	m_data[2][0] = m[2][0]; m_data[2][1] = m[2][1]; m_data[2][2] = m[0][0];
}

float* Matrix3::operator[]( int i )
{
	return m_data[i];
}

const float* Matrix3::operator[]( int i ) const
{
	return m_data[i];
}

Matrix3 Matrix3::transposed() const
{
    Matrix3 result;
    
    result[0][0] = m_data[0][0];
    result[0][1] = m_data[1][0];
    result[0][2] = m_data[2][0];
    
    result[1][0] = m_data[0][1];
    result[1][1] = m_data[1][1];
    result[1][2] = m_data[2][1];
    
    result[2][0] = m_data[0][2];
    result[2][1] = m_data[1][2];
    result[2][2] = m_data[2][2];
    
    return result;
}

Matrix3 Matrix3::inverse() const
{
    Matrix3 result;
    
    float det = m_data[0][0] * ( m_data[1][1] * m_data[2][2] - m_data[1][2] * m_data[2][1] )
              - m_data[0][1] * ( m_data[1][0] * m_data[2][2] - m_data[1][2] * m_data[2][0] )
              + m_data[0][2] * ( m_data[1][0] * m_data[2][1] - m_data[1][1] * m_data[2][0] );

    // The determinant must not be zero.
    if ( det == 0.0f ) return Matrix3::Identity;
    
    float invDet = 1.0f / det;

    result[0][0] =  ( m_data[1][1] * m_data[2][2] - m_data[1][2] * m_data[2][1] ) * invDet;
    result[0][1] = -( m_data[0][1] * m_data[2][2] - m_data[0][2] * m_data[2][1] ) * invDet;
    result[0][2] =  ( m_data[0][1] * m_data[1][2] - m_data[0][2] * m_data[1][1] ) * invDet;

    result[1][0] = -( m_data[1][0] * m_data[2][2] - m_data[1][2] * m_data[2][0] ) * invDet;
    result[1][1] =  ( m_data[0][0] * m_data[2][2] - m_data[0][2] * m_data[2][0] ) * invDet;
    result[1][2] = -( m_data[0][0] * m_data[1][2] - m_data[0][2] * m_data[1][0] ) * invDet;

    result[2][0] =  ( m_data[1][0] * m_data[2][1] - m_data[1][1] * m_data[2][0] ) * invDet;
    result[2][1] = -( m_data[0][0] * m_data[2][1] - m_data[0][1] * m_data[2][0] ) * invDet;
    result[2][2] =  ( m_data[0][0] * m_data[1][1] - m_data[0][1] * m_data[1][0] ) * invDet;
    
    return result;
}

Vector Matrix3::transformPoint( const Vector& point ) const
{
    Vector result;

    result.x = m_data[0][0] * point.x + m_data[0][1] * point.y + m_data[0][2] * point.z;
    result.y = m_data[1][0] * point.x + m_data[1][1] * point.y + m_data[1][2] * point.z;
    result.z = m_data[2][0] * point.x + m_data[2][1] * point.y + m_data[2][2] * point.z;

    return result;
}

