// Oleg Kotov

#pragma once

#include "vertex.h"
#include "triangle.h"

#include <vector>

struct Mesh
{
    std::vector<Vertex> vertices;
    std::vector<Triangle> triangles;
};

Mesh createCubeMesh();
Mesh createTriangleMesh();

