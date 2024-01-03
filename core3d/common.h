#ifndef CORE3D_COMMON_H
#define CORE3D_COMMON_H

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace Core3d {

    std::vector<glm::vec3> computeNormals(
            const std::vector<glm::vec3> &vertices,
            const std::vector<int> &triangleVertexIndices);

    std::vector<int> triangulate(
            const std::vector<int> &polygonVertexIndices,
            const std::vector<int> &polygonStarts);

    std::vector<glm::vec3> computeTangentSpace(
                const std::vector<glm::vec3> &vertices,
                const std::vector<int> &triangleVertexIndices,
                const std::vector<glm::vec2> &texcoords,
                const std::vector<int> &triangleTexcoordIndices);
}

#endif
