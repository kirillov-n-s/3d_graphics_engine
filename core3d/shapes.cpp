#include <cmath>
#include <glm/ext/scalar_constants.hpp>
#include "shapes.h"

namespace Core3d::Shapes {

    TriangleMesh makeUvSphere(
        const float radius,
        const int nRows, const int nCols)
    {
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> texcoords;
        std::vector<glm::vec3> normals;
        std::vector<int> vertexAndTexcoordIndices;

        constexpr auto pi = glm::pi<float>();

        const float rowStep = pi / nRows;
        const float colStep = 2.0f * pi / nCols;
        const float invRadius = 1.0f / radius;

        for (int rowInd = 0; rowInd <= nRows; ++rowInd)
        {
            const float rowAngle = pi * 0.5f - rowInd * rowStep;
            const float xz = radius * std::cos(rowAngle);
            const float y = -radius * std::sin(rowAngle);

            for (int colInd = 0; colInd <= nCols; ++colInd)
            {
                const float colAngle = colInd * colStep;
                const float x = xz * std::cos(colAngle);
                const float z = xz * std::sin(colAngle);

                vertices.emplace_back(x, y, z);
                texcoords.emplace_back(
                    static_cast<float>(colInd) / nCols,
                    static_cast<float>(rowInd) / nRows);
                normals.emplace_back(x * invRadius, y * invRadius, z * invRadius);
            }
        }

        /*
         *  t--t+1
         *  |  / |
         *  | /  |
         *  b--b+1
         */
        for (int rowInd = 0; rowInd < nRows; ++rowInd)
        {
            int topInd = rowInd * (nCols + 1);
            int bottomInd = topInd + nCols + 1;

            for (int colInd = 0; colInd < nCols; ++colInd, ++topInd, ++bottomInd)
            {
                if (rowInd != 0) {
                    vertexAndTexcoordIndices.push_back(topInd);
                    vertexAndTexcoordIndices.push_back(bottomInd);
                    vertexAndTexcoordIndices.push_back(topInd + 1);
                }

                if (rowInd != (nRows - 1)) {
                    vertexAndTexcoordIndices.push_back(topInd + 1);
                    vertexAndTexcoordIndices.push_back(bottomInd);
                    vertexAndTexcoordIndices.push_back(bottomInd + 1);
                }
            }
        }

        // remove isolated vertices (first and last)
        vertices.erase(vertices.begin());
        vertices.erase(vertices.end() - 1);
        texcoords.erase(texcoords.begin());
        texcoords.erase(texcoords.end() - 1);
        normals.erase(normals.begin());
        normals.erase(normals.end() - 1);
        for (int &index : vertexAndTexcoordIndices)
            index -= 1;

        return TriangleMesh(
            vertices,
            vertexAndTexcoordIndices,
            normals,
            texcoords,
            vertexAndTexcoordIndices);
    }

}
