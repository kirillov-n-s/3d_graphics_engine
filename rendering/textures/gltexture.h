#ifndef RENDERING_TEXTURES_GLTEXTURE_H
#define RENDERING_TEXTURES_GLTEXTURE_H

#include <string>

namespace Rendering::Textures {

    class GlTexture
    {
    public:
        explicit GlTexture(const std::string &path);
        ~GlTexture();

        void use(const int textureUnit = 0) const;

    private:
        unsigned int m_id = 0;
    };
}

#endif
