#ifndef RENDERING_TEXTURES_ENVIRONMENT_H
#define RENDERING_TEXTURES_ENVIRONMENT_H

#include <memory>
#include <array>
#include "../../core2d/image.h"

namespace Rendering::Textures {

    std::array<std::shared_ptr<Core2d::Image>, 6> equirectangularMapToCubemapFaces(
        const Core2d::Image &equirectangularMap,
        const int cubeDim);

    std::array<std::shared_ptr<Core2d::Image>, 6> cubemapToIrradianceMap(
        const std::array<std::shared_ptr<Core2d::Image>, 6> &cubeFaces,
        const int irradianceCubeDim);
}

#endif
