#ifndef CORE3D_TRIANGLE_MESH_H
#define CORE3D_TRIANGLE_MESH_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace Core3d {
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
            std::vector<int> texcoordIndices = {},
            std::vector<glm::vec3> tangents = {});

        bool hasTexcoords() const;
        bool hasNormals() const;
        bool hasTangents() const;

        int nVertices() const;
        int nTexcoords() const;
        int nTriangles() const;

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texcoords;
        std::vector<glm::vec3> tangents;

        std::vector<int> vertexIndices;
        std::vector<int> texcoordIndices;
    };
}

#endif
