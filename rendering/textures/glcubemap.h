#ifndef RENDERING_TEXTURES_GLCUBEMAP_H
#define RENDERING_TEXTURES_GLCUBEMAP_H

#include <memory>
#include <array>
#include "../../core2d/image.h"
#include "../meshes/glcube.h"

namespace Rendering::Textures {

    class GlCubemap
    {
    public:
        explicit GlCubemap(
            const std::array<std::shared_ptr<Core2d::Image>, 6> &cubeFaces,
            const bool generateMipmap = false);
        ~GlCubemap();

        void use(const int textureUnit = 0) const;
        void draw(const int textureUnit = 0) const;

    private:
        unsigned int m_textureId;
        Meshes::GlCube m_glCube;
    };
}

#endif
