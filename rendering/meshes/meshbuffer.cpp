#include <unordered_map>
#include "meshbuffer.h"

namespace Rendering::Meshes {

    MeshIndex makeIndex(const Core3d::TriangleMesh &mesh, const int flatIndex)
    {
        return {
                .vertex = mesh.vertexIndices[flatIndex],
                .texcoord = mesh.hasTexcoords()
                    ? mesh.texcoordIndices[flatIndex]
                    : -1,
                .normal = mesh.hasNormals()
                    ? mesh.vertexIndices[flatIndex]
                    : -1
        };
    }

    MeshPoint pointAtIndex(const Core3d::TriangleMesh &mesh, const MeshIndex &index)
    {
        return {
                .vertex = mesh.vertices[index.vertex],
                .texcoord = index.texcoord != -1
                    ? mesh.texcoords[index.texcoord]
                    : glm::vec3(0.f),
                .normal = index.normal != -1
                    ? mesh.normals[index.normal]
                    : glm::vec3(0.f)
        };
    }

    bool operator==(const MeshIndex &lhs, const MeshIndex &rhs)
    {
        return lhs.vertex == rhs.vertex
               && lhs.texcoord == rhs.texcoord
               && lhs.normal == rhs.normal;
    }

    bool operator!=(const MeshIndex &lhs, const MeshIndex &rhs)
    {
        return !(lhs == rhs);
    }

    bool operator==(const MeshPoint &lhs, const MeshPoint &rhs)
    {
        return lhs.vertex == rhs.vertex
               && lhs.texcoord == rhs.texcoord
               && lhs.normal == rhs.normal;
    }

    bool operator!=(const MeshPoint &lhs, const MeshPoint &rhs)
    {
        return !(lhs == rhs);
    }

    MeshBuffer::MeshBuffer(const Core3d::TriangleMesh &mesh)
    {
        std::unordered_map<MeshIndex, unsigned int> indToElemMap;
        const int nFlatIndices = mesh.vertexIndices.size();
        for (int flatInd = 0; flatInd < nFlatIndices; flatInd++) {

            const MeshIndex& index = makeIndex(mesh, flatInd);

            const auto &mapPairIt = indToElemMap.find(index);
            if (mapPairIt != indToElemMap.end()) {
                elements.push_back(mapPairIt->second);
                continue;
            }

            const unsigned int elem = indToElemMap.size();
            indToElemMap[index] = elem;

            points.push_back(pointAtIndex(mesh, index));
            indices.push_back(index);
            elements.push_back(elem);
        }
    }

    void MeshBuffer::updateGeometry(
            const std::vector<glm::vec3> &vertices,
            const std::vector<glm::vec3> &normals)
    {
        const int nPoints = points.size();
        for (int ind = 0; ind < nPoints; ++ind) {

            const MeshIndex &index = indices[ind];
            MeshPoint &point = points[ind];

            point.vertex = vertices[index.vertex];
            point.normal = normals[index.normal];
        }
    }
}
