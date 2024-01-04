#include <gl/glew.h>
#include "glcubemap.h"


namespace Rendering::Textures {

    GlCubemap::GlCubemap(const std::array<std::shared_ptr<Core2d::Image>, 6> &cubeFaces)
    {
        glGenTextures(1, &m_textureId);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);

        for (int faceInd = 0; faceInd < cubeFaces.size(); ++faceInd) {
            const std::shared_ptr<Core2d::Image> &cubeFace = cubeFaces[faceInd];
            const int glFormat = Core2d::formatToGlFormat(cubeFace->format());
            const int glInternalFormat = Core2d::formatToGlInternalFormat(cubeFace->format());
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceInd,
                0,
                glInternalFormat,
                cubeFace->width(), cubeFace->height(),
                0,
                glFormat,
                static_cast<int>(cubeFace->dtype()),
                cubeFace->data());
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    GlCubemap::~GlCubemap()
    {
        glDeleteTextures(1, &m_textureId);
    }

    void GlCubemap::draw(const int textureUnit) const
    {
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        glDisable(GL_CULL_FACE);
        glDepthMask(GL_FALSE);

        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);

        m_glCube.draw();

        glDepthMask(GL_TRUE);
        glEnable(GL_CULL_FACE);
        glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    }
}
