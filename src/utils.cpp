// Oleg Kotov

#include "utils.h"
#include "vector.h"

uint16_t rgb565( uint8_t r, uint8_t g, uint8_t b )
{
    return ( ( r & 0xF8 ) << 8 ) | ( ( g & 0xFC ) << 3 ) | ( b >> 3 );
}

Vector calcTriangleNormal( const Vector& v0, const Vector& v1, const Vector& v2 )
{
    Vector e1 = v1 - v0;
    Vector e2 = v2 - v0;

    return Vector::crossProduct( e1, e2 ).normalize();
}

