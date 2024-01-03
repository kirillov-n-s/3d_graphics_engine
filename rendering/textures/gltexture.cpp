#include <iostream>
#include <gl/glew.h>
#include "../../stb_image/stb_image.h"
#include "gltexture.h"

namespace Rendering::Textures {

    GlTexture::GlTexture(const std::string &path)
    {
        stbi_set_flip_vertically_on_load(true);
        int width, height, channels;
        auto* data = stbi_load(
            path.c_str(),
            &width, &height, &channels,
            0);
        int format = -1;
        switch (channels) {
            case 1:
                format = GL_RED;
            break;
            case 2:
                format = GL_RG;
            break;
            case 3:
                format = GL_RGB;
            break;
            case 4:
                format = GL_RGBA;
            break;
            default:
                std::cerr
                    << "Got " << channels << " channels while loading image "
                    << path << std::endl;
                exit(228);
        }

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            format,  // bug srgb?
            width, height,
            0,
            format,
            GL_UNSIGNED_BYTE,
            data);

        stbi_image_free(data);
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
