#ifndef RENDERING_MESHES_MESHBUFFER_H
#define RENDERING_MESHES_MESHBUFFER_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "../../geometry/trianglemesh.h"

namespace Rendering::Meshes {

    struct MeshPoint
    {
        glm::vec3 vertex = glm::vec3(0.0f);
        glm::vec2 texcoord = glm::vec2(0.0f);
        glm::vec3 normal = glm::vec3(0.0f);
    };

    struct MeshIndex
    {
        int vertex = -1;
        int texcoord = -1;
        int normal = -1;
    };

    MeshIndex makeIndex(const Geometry::TriangleMesh &mesh, const int flatIndex);

    MeshPoint pointAtIndex(const Geometry::TriangleMesh &mesh, const MeshIndex &index);

    bool operator==(const MeshIndex &lhs, const MeshIndex &rhs);
    bool operator!=(const MeshIndex &lhs, const MeshIndex &rhs);

    bool operator==(const MeshPoint &lhs, const MeshPoint &rhs);
    bool operator!=(const MeshPoint &lhs, const MeshPoint &rhs);

    class MeshBuffer
    {
    public:
        explicit MeshBuffer(const Geometry::TriangleMesh &mesh);

        void updateGeometry(
                const std::vector<glm::vec3> &vertices,
                const std::vector<glm::vec3> &normals);

        std::vector<MeshPoint> points;
        std::vector<MeshIndex> indices;
        std::vector<unsigned int> elements;
    };
}

template<>
struct std::hash<Rendering::Meshes::MeshIndex>
{
    size_t operator()(const Rendering::Meshes::MeshIndex &index) const noexcept
    {
        return std::hash<int>{}(index.vertex)
               ^ std::hash<int>{}(index.texcoord)
               ^ std::hash<int>{}(index.normal);
    }
};

#endif
