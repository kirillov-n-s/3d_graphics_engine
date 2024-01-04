#include <gl/glew.h>
#include "glcube.h"

namespace Rendering::Meshes {

    GlCube::GlCube()
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
            3 * sizeof(float),
            nullptr);
        glEnableVertexAttribArray(0);
    }

    GlCube::~GlCube()
    {
        glDeleteBuffers(1, &m_vertexBufferObject);
        glDeleteVertexArrays(1, &m_vertexArrayObject);
    }

    void GlCube::draw() const
    {
        glBindVertexArray(m_vertexArrayObject);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

}
