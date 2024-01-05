#include <vector>
#include "../../common/debug.h"
#include "../shaders/glshaderprogram.h"
#include "../meshes/glcube.h"
#include "gltexture.h"
#include "glcubemap.h"
#include "rendertocubemap.h"
#include "environment.h"

namespace Rendering::Textures {

    std::array<std::shared_ptr<Core2d::Image>, 6> equirectangularMapToCubemapFaces(
        const Core2d::Image &equirectangularMap,
        const int cubeDim)
    {
        const Core2d::ImageFormat mapFormat = equirectangularMap.format();
        const Core2d::ImageDType mapDtype = equirectangularMap.dtype();

        const int glInternalFormat = Core2d::formatToGlInternalFormat(mapFormat);
        const int glFormat = Core2d::formatToGlFormat(mapFormat);
        const int glDtype = static_cast<int>(mapDtype);

        const GlTexture equirectangularTexture(equirectangularMap, false);

        const RenderToCubemap renderToCubemap(
            cubeDim, glInternalFormat, glFormat, glDtype);

        std::string error;

        const Shaders::GlShader vertShader(
            "../data/shaders/equirectangular_map.vert",
            Shaders::GlShaderType::VertexShader,
            error);
        Common::exitOnError(error, 1337);

        const Shaders::GlShader fragShader(
            "../data/shaders/equirectangular_map.frag",
            Shaders::GlShaderType::FragmentShader,
            error);
        Common::exitOnError(error, 1338);

        const Shaders::GlShaderProgram shaderProgram(
            {vertShader, fragShader},
            error);
        Common::exitOnError(error, 1339);

        const Meshes::GlCube cube;

        shaderProgram.use();
        shaderProgram.setUniform("uEquirectangularMap", 0);
        shaderProgram.setUniform("uProjection", cubemapCaptureProjection);
        equirectangularTexture.use(0);

        renderToCubemap.useFramebuffer();

        std::array<std::shared_ptr<Core2d::Image>, 6> cubeFaces;
        for (int faceInd = 0; faceInd < 6; ++faceInd) {
            shaderProgram.setUniform("uView", cubemapCaptureViews[faceInd]);
            renderToCubemap.useFramebufferTexture(faceInd);
            cube.draw();
            cubeFaces[faceInd] = std::make_shared<Core2d::Image>(cubeDim, cubeDim, mapFormat);
            renderToCubemap.readFramebuffer(cubeFaces[faceInd]->data());
        }
        return cubeFaces;
    }

    std::array<std::shared_ptr<Core2d::Image>, 6> cubemapToIrradianceMap(
        const std::array<std::shared_ptr<Core2d::Image>, 6> &cubeFaces,
        const int irradianceCubeDim)
    {
        const Core2d::ImageFormat mapFormat = cubeFaces[0]->format();
        const Core2d::ImageDType mapDtype = cubeFaces[0]->dtype();

        const int glInternalFormat = Core2d::formatToGlInternalFormat(mapFormat);
        const int glFormat = Core2d::formatToGlFormat(mapFormat);
        const int glDtype = static_cast<int>(mapDtype);

        const GlCubemap envCubemap(cubeFaces);

        const RenderToCubemap renderToCubemap(
            irradianceCubeDim, glInternalFormat, glFormat, glDtype);

        std::string error;

        const Shaders::GlShader vertShader(
            "../data/shaders/cubemap_convolution.vert",
            Shaders::GlShaderType::VertexShader,
            error);
        Common::exitOnError(error, 1337);

        const Shaders::GlShader fragShader(
            "../data/shaders/cubemap_convolution.frag",
            Shaders::GlShaderType::FragmentShader,
            error);
        Common::exitOnError(error, 1338);

        const Shaders::GlShaderProgram shaderProgram(
            {vertShader, fragShader},
            error);
        Common::exitOnError(error, 1339);

        shaderProgram.use();
        shaderProgram.setUniform("uEnvironment", 0);
        shaderProgram.setUniform("uProjection", cubemapCaptureProjection);

        renderToCubemap.useFramebuffer();

        std::array<std::shared_ptr<Core2d::Image>, 6> irradianceCubeFaces;
        for (int faceInd = 0; faceInd < 6; ++faceInd) {
            shaderProgram.setUniform("uView", cubemapCaptureViews[faceInd]);
            renderToCubemap.useFramebufferTexture(faceInd);
            envCubemap.draw();
            irradianceCubeFaces[faceInd] = std::make_shared<Core2d::Image>(
                irradianceCubeDim, irradianceCubeDim, mapFormat);
            renderToCubemap.readFramebuffer(irradianceCubeFaces[faceInd]->data());
        }
        return irradianceCubeFaces;
    }
}
