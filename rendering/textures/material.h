#ifndef RENDERING_TEXTURES_MATERIAL_H
#define RENDERING_TEXTURES_MATERIAL_H

#include <memory>
#include "../../core2d/image.h"
#include "../meshes/glmesh.h"

namespace Rendering::Textures {

    std::shared_ptr<Core2d::Image> heightMapToNormalMap(
        const std::shared_ptr<Meshes::GlMesh> &mesh,
        const Core2d::Image &heightMap,
        const float heightScale = 1.0f,
        const int nSmoothIters = 0);

    std::shared_ptr<Core2d::Image> gaussianBlur(const Core2d::Image &image);
}

#endif
