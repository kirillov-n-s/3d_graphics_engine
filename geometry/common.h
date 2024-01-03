#ifndef GEOMETRY_COMMON_H
#define GEOMETRY_COMMON_H

#include <vector>
#include <glm/vec3.hpp>

namespace Geometry {

    std::vector<glm::vec3> computeNormals(
            const std::vector<glm::vec3> &vertices,
            const std::vector<int> &triangleVertexIndices);

    std::vector<int> triangulate(
            const std::vector<int> &polygonVertexIndices,
            const std::vector<int> &polygonStarts);
}

#endif
