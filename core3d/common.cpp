#include <glm/geometric.hpp>
#include "../common/common.h"
#include "common.h"

namespace Core3d {

    std::vector<glm::vec3> computeNormals(
            const std::vector<glm::vec3> &vertices,
            const std::vector<int> &triangleVertexIndices)
    {
        assert(triangleVertexIndices.size() % 3 == 0);

        const int nVertices = vertices.size();
        std::vector<glm::vec3> normals(nVertices, glm::vec3(0.0f));
        std::vector<int> nTrianglesByVertex(nVertices, 0);

        const int nTriangles = triangleVertexIndices.size() / 3;
        for (int triangleInd = 0; triangleInd < nTriangles; ++triangleInd) {

            const int aInd = triangleVertexIndices[triangleInd * 3 + 0];
            const int bInd = triangleVertexIndices[triangleInd * 3 + 1];
            const int cInd = triangleVertexIndices[triangleInd * 3 + 2];

            assert(aInd != bInd);
            assert(aInd != cInd);
            assert(bInd != cInd);

            ++nTrianglesByVertex[aInd];
            ++nTrianglesByVertex[bInd];
            ++nTrianglesByVertex[cInd];

            const glm::vec3 &a = vertices[aInd];
            const glm::vec3 &b = vertices[bInd];
            const glm::vec3 &c = vertices[cInd];

            const glm::vec3 normal = glm::normalize(glm::cross(b - a, c - a));

            normals[aInd] += normal;
            normals[bInd] += normal;
            normals[cInd] += normal;
        }

        for (int vertexInd = 0; vertexInd < nVertices; ++vertexInd) {

            assert(nTrianglesByVertex[vertexInd] != 0);
            normals[vertexInd] /= nTrianglesByVertex[vertexInd];
            //assert(Common::fuzzyCompare(glm::length(normals[vertexInd]), 1.0f));
        }

        return normals;
    }

    std::vector<int> triangulate(
            const std::vector<int> &polygonVertexIndices,
            const std::vector<int> &polygonStarts)
    {
        std::vector<int> triangleVertexIndices;
        triangleVertexIndices.reserve(polygonVertexIndices.size());

        const int nPolygons = polygonStarts.size() - 1;
        for (int polygonInd = 0; polygonInd < nPolygons; ++polygonInd) {

            const int faceBegin = polygonStarts[polygonInd];
            const int faceEnd = polygonStarts[polygonInd + 1] - 1;
            for (int faceInd = faceBegin + 1; faceInd < faceEnd; ++faceInd) {

                triangleVertexIndices.push_back(polygonVertexIndices[faceBegin]);
                triangleVertexIndices.push_back(polygonVertexIndices[faceInd]);
                triangleVertexIndices.push_back(polygonVertexIndices[faceInd + 1]);
            }
        }

        return triangleVertexIndices;
    }

    std::vector<glm::vec3> computeTangentSpace(
            const std::vector<glm::vec3> &vertices,
            const std::vector<int> &triangleVertexIndices,
            const std::vector<glm::vec2> &texcoords,
            const std::vector<int> &triangleTexcoordIndices)
    {
        assert(triangleVertexIndices.size() % 3 == 0);
        assert(triangleVertexIndices.size() == triangleTexcoordIndices.size());

        const int nVertices = vertices.size();
        std::vector<glm::vec3> tangents(nVertices, glm::vec3(0.0f));
        std::vector<int> nTrianglesByVertex(nVertices, 0);

        const int nTriangles = triangleVertexIndices.size() / 3;
        for (int triangleInd = 0; triangleInd < nTriangles; ++triangleInd) {

            const int aVertInd = triangleVertexIndices[triangleInd * 3 + 0];
            const int bVertInd = triangleVertexIndices[triangleInd * 3 + 1];
            const int cVertInd = triangleVertexIndices[triangleInd * 3 + 2];

            ++nTrianglesByVertex[aVertInd];
            ++nTrianglesByVertex[bVertInd];
            ++nTrianglesByVertex[cVertInd];

            const glm::vec3 &aVert = vertices[aVertInd];
            const glm::vec3 &bVert = vertices[bVertInd];
            const glm::vec3 &cVert = vertices[cVertInd];

            const glm::vec3 edge1 = bVert - aVert;
            const glm::vec3 edge2 = cVert - aVert;

            const int aTexInd = triangleTexcoordIndices[triangleInd * 3 + 0];
            const int bTexInd = triangleTexcoordIndices[triangleInd * 3 + 1];
            const int cTexInd = triangleTexcoordIndices[triangleInd * 3 + 2];

            const glm::vec2 &aTex = texcoords[aTexInd];
            const glm::vec2 &bTex = texcoords[bTexInd];
            const glm::vec2 &cTex = texcoords[cTexInd];

            const glm::vec2 deltaTex1 = bTex - aTex;
            const glm::vec2 deltaTex2 = cTex - aTex;

            const float invDet = 1.0f / (deltaTex1.x * deltaTex2.y - deltaTex2.x * deltaTex1.y);

            const glm::vec3 tangent = invDet * (deltaTex2.y * edge1 - deltaTex1.y * edge2);

            tangents[aVertInd] += tangent;
            tangents[bVertInd] += tangent;
            tangents[cVertInd] += tangent;
        }

        for (int vertexInd = 0; vertexInd < nVertices; ++vertexInd) {
            assert(nTrianglesByVertex[vertexInd] != 0);
            tangents[vertexInd] /= nTrianglesByVertex[vertexInd];
        }

        return tangents;
    }
}