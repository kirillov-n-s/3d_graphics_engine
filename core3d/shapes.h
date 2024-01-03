#ifndef CORE3D_SHAPES_H
#define CORE3D_SHAPES_H

#include "trianglemesh.h"

namespace Core3d::Shapes {

    TriangleMesh makeUvSphere(
        const float radius,
        const int nRows, const int nCols);
}

#endif
