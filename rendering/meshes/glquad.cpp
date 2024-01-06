#include <gl/glew.h>
#include "glquad.h"

namespace Rendering::Meshes {

    GlQuad::GlQuad()
    {
        glGenVertexArrays(1, &m_vertexArrayObject);
        glBindVertexArray(m_vertexArrayObject);

        glGenBuffers(1, &m_vertexBufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(vertices),
            vertices,
            GL_STATIC_DRAW);

        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            5 * sizeof(float),
            nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            1,
            2,
            GL_FLOAT,
            GL_FALSE,
            5 * sizeof(float),
            reinterpret_cast<void *>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    GlQuad::~GlQuad()
    {
        glDeleteBuffers(1, &m_vertexBufferObject);
        glDeleteVertexArrays(1, &m_vertexArrayObject);
    }

    void GlQuad::draw() const
    {
        glBindVertexArray(m_vertexArrayObject);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

}
