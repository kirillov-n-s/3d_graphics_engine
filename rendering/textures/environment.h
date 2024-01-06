#ifndef RENDERING_TEXTURES_ENVIRONMENT_H
#define RENDERING_TEXTURES_ENVIRONMENT_H

#include <memory>
#include <array>
#include "../../core2d/image.h"

namespace Rendering::Textures {

    const glm::mat4 cubemapCaptureProjection = glm::perspective(
        glm::radians(90.0f),
        1.0f,
        0.1f, 10.0f);

    const glm::mat4 cubemapCaptureViews[6] = {
        glm::lookAt(
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3( 1.0f,  0.0f,  0.0f),
            glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(-1.0f,  0.0f,  0.0f),
            glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3( 0.0f,  1.0f,  0.0f),
            glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3( 0.0f, -1.0f,  0.0f),
            glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3( 0.0f,  0.0f,  1.0f),
            glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3( 0.0f,  0.0f, -1.0f),
            glm::vec3(0.0f, -1.0f,  0.0f))
    };

    std::array<std::shared_ptr<Core2d::Image>, 6> equirectangularMapToCubemapFaces(
        const Core2d::Image &equirectangularMap,
        const int cubeDim);

    std::array<std::shared_ptr<Core2d::Image>, 6> cubemapToIrradianceMap(
        const std::array<std::shared_ptr<Core2d::Image>, 6> &cubeFaces,
        const int irradianceCubeDim);

    std::shared_ptr<GlCubemap> prefilterCubemap(
        const std::array<std::shared_ptr<Core2d::Image>, 6> &cubeFaces,
        const int prefilteredCubeDim, const int prefilteredMipLevels);

    std::shared_ptr<GlTexture> integrateBrdf(const int lutTextureDim);
}

#endif
