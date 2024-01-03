#ifndef GEOMETRY_TRIANGLE_MESH_H
#define GEOMETRY_TRIANGLE_MESH_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace Geometry {
    class TriangleMesh
    {
    public:
        TriangleMesh() = default;
        TriangleMesh(const TriangleMesh &) = default;

        TriangleMesh(
            std::vector<glm::vec3> vertices,
            std::vector<int> vertexIndices,
            std::vector<glm::vec3> normals = {},
            std::vector<glm::vec2> texcoords = {},
            std::vector<int> texcoordIndices = {});

        bool hasTexcoords() const;
        bool hasNormals() const;

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texcoords;

        std::vector<int> vertexIndices;
        std::vector<int> texcoordIndices;
    };
}

#endif
