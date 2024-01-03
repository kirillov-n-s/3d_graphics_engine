#include "common.h"
#include "trianglemesh.h"

namespace Core3d {

    TriangleMesh::TriangleMesh(
        std::vector<glm::vec3> vertices,
        std::vector<int> vertexIndices,
        std::vector<glm::vec3> normals,
        std::vector<glm::vec2> texcoords,
        std::vector<int> texcoordIndices)
    : vertices(std::move(vertices)),
      vertexIndices(std::move(vertexIndices)),
      normals(std::move(normals)),
      texcoords(std::move(texcoords)),
      texcoordIndices(std::move(texcoordIndices))
    {
        if (this->normals.empty())
            this->normals = computeNormals(this->vertices, this->vertexIndices);
    }

    bool TriangleMesh::hasTexcoords() const
    {
        return !texcoords.empty() && !texcoordIndices.empty();
    }

    bool TriangleMesh::hasNormals() const
    {
        return !normals.empty();
    }
}