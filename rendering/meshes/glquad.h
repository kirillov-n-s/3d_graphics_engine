#ifndef RENDERING_MESHES_GLQUAD_H
#define RENDERING_MESHES_GLQUAD_H

namespace Rendering::Meshes {

    class GlQuad
    {
    public:
        static constexpr float vertices[] = {
            -1.0f, 1.0f, 0.0f,
            0.0f, 1.0f,

            -1.0f, -1.0f, 0.0f,
            0.0f, 0.0f,

            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f,

            1.0f, -1.0f, 0.0f,
            1.0f, 0.0f,
        };

        GlQuad();
        ~GlQuad();

        void draw() const;

    private:
        unsigned int m_vertexBufferObject;
        unsigned int m_vertexArrayObject;
    };
}

#endif
