#ifndef RENDERING_MESHES_GLMESH_H
#define RENDERING_MESHES_GLMESH_H

#include "meshbuffer.h"

namespace Rendering::Meshes {

    class GlMesh
    {
    public:
        explicit GlMesh(const MeshBuffer &buffer);
        ~GlMesh();

        void draw(
            const bool useTesselation = false,
            const bool drawWireframe = false) const;

        void swapVertexBuffer(const MeshBuffer &buffer) const;

    private:
        int m_nElements = -1;

        unsigned int m_vertexBufferObject = 0;
        unsigned int m_vertexArrayObject = 0;
        unsigned int m_elementBufferObject = 0;
    };
}

#endif
