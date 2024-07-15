// Oleg Kotov

#pragma once

class Vector;

#include <stdint.h>

uint16_t rgb565( uint8_t r, uint8_t g, uint8_t b );
Vector calcTriangleNormal( const Vector& v0, const Vector& v1, const Vector& v2 );

