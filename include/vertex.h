// Oleg Kotov

#pragma once

#include "vector.h"
#include <stdint.h>

struct Vertex
{
    Vector position;
    Vector normal;
    float uv[2];
    Vector color;

    Vector positionWS; // unused
    Vector outPosition; // unused
    uint16_t outColor; // unused
};

