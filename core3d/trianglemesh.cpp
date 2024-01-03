#include "common.h"
#include "trianglemesh.h"

namespace Core3d {

    TriangleMesh::TriangleMesh(
        std::vector<glm::vec3> vertices,
        std::vector<int> vertexIndices,
        std::vector<glm::vec3> normals,
        std::vector<glm::vec2> texcoords,
        std::vector<int> texcoordIndices,
        std::vector<glm::vec3> tangents)
    : vertices(std::move(vertices)),
      vertexIndices(std::move(vertexIndices)),
      normals(std::move(normals)),
      texcoords(std::move(texcoords)),
      texcoordIndices(std::move(texcoordIndices)),
      tangents(std::move(tangents))
    {
        if (this->normals.empty())
            this->normals = computeNormals(this->vertices, this->vertexIndices);
        if (this->tangents.empty() && hasTexcoords())
            this->tangents = computeTangentSpace(
                this->vertices, this->vertexIndices,
                this->texcoords, this->texcoordIndices);
    }

    bool TriangleMesh::hasTexcoords() const
    {
        return !texcoords.empty() && !texcoordIndices.empty();
    }

    bool TriangleMesh::hasNormals() const
    {
        return !normals.empty();
    }

    bool TriangleMesh::hasTangents() const
    {
        return !tangents.empty();
    }

    int TriangleMesh::nVertices() const
    {
        return vertices.size();
    }

    int TriangleMesh::nTexcoords() const
    {
        return texcoords.size();
    }

    int TriangleMesh::nTriangles() const
    {
        return vertexIndices.size() / 3;
    }
}
