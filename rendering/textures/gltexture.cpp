#include <iostream>
#include <gl/glew.h>
#include "../../io/stb_image/stb_image.h"
#include "gltexture.h"

namespace Rendering::Textures {

    GlTexture::GlTexture(const Core2d::Image &image, const bool useSrgb)
    {
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        const int glImageFormat = Core2d::formatToGlValue(image.format());
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            useSrgb ? GL_SRGB : glImageFormat,
            image.width(), image.height(),
            0,
            glImageFormat,
            GL_UNSIGNED_BYTE,
            image.data());
    }

    GlTexture::~GlTexture()
    {
        glDeleteTextures(1, &m_id);
    }

    void GlTexture::use(const int textureUnit) const
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }


}
