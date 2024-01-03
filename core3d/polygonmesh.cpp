#include <utility>
#include "../Common/Common.h"
#include "common.h"
#include "polygonmesh.h"

namespace Core3d {

    PolygonMesh::PolygonMesh(
            std::vector<glm::vec3> vertices,
            std::vector<int> vertexIndices,
            std::vector<int> polygonStarts,
            std::vector<glm::vec3> normals,
            std::vector<glm::vec2> texcoords,
            std::vector<int> texcoordIndices)
        : vertices(std::move(vertices)),
          vertexIndices(std::move(vertexIndices)),
          polygonStarts(std::move(polygonStarts)),
          normals(std::move(normals)),
          texcoords(std::move(texcoords)),
          texcoordIndices(std::move(texcoordIndices))
    {}

    PolygonMesh::PolygonMesh(const TriangleMesh &triangleMesh)
        : vertices(triangleMesh.vertices),
          vertexIndices(triangleMesh.vertexIndices),
          polygonStarts(makeTriangleStarts(triangleMesh.nTriangles())),
          normals(triangleMesh.normals),
          texcoords(triangleMesh.texcoords),
          texcoordIndices(triangleMesh.texcoordIndices)
    {}

    bool PolygonMesh::hasTexcoords() const
    {
        return !texcoords.empty() && !texcoordIndices.empty();
    }

    bool PolygonMesh::hasNormals() const
    {
        return !normals.empty();
    }
}
