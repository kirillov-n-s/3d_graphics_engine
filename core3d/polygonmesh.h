#ifndef CORE3D_POLYGON_MESH_H
#define CORE3D_POLYGON_MESH_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "trianglemesh.h"

namespace Core3d {

    class PolygonMesh
    {
    public:
        PolygonMesh() = default;
        PolygonMesh(const PolygonMesh &) = default;

        PolygonMesh(
                std::vector<glm::vec3> vertices,
                std::vector<int> vertexIndices,
                std::vector<int> polygonStarts,
                std::vector<glm::vec3> normals = {},
                std::vector<glm::vec2> texcoords = {},
                std::vector<int> texcoordIndices = {});

        explicit PolygonMesh(const TriangleMesh &triangleMesh);

        bool hasTexcoords() const;
        bool hasNormals() const;

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texcoords;

        std::vector<int> vertexIndices;
        std::vector<int> texcoordIndices;

        std::vector<int> polygonStarts;
    };
}

#endif
