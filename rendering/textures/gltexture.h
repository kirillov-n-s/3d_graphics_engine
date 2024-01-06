#ifndef RENDERING_TEXTURES_GLTEXTURE_H
#define RENDERING_TEXTURES_GLTEXTURE_H

#include <string>
#include "../../core2d/image.h"

namespace Rendering::Textures {

    class GlTexture
    {
    public:
        explicit GlTexture(const Core2d::Image &image, const bool useSrgb = false);
        explicit GlTexture(const unsigned int preallocatedTextureId);
        ~GlTexture();

        void use(const int textureUnit = 0) const;

    private:
        unsigned int m_id = 0;
    };
}

#endif
