#include <vector>
#include "../../common/debug.h"
#include "../shaders/glshaderprogram.h"
#include "../meshes/glcube.h"
#include "gltexture.h"
#include "glcubemap.h"
#include "rendertocubemap.h"
#include "environment.h"

#include "rendertotexture.h"
#include "../meshes/glquad.h"

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

    std::shared_ptr<GlCubemap> prefilterCubemap(
        const std::array<std::shared_ptr<Core2d::Image>, 6> &cubeFaces,
        const int prefilteredCubeDim, const int prefilteredMipLevels)
    {
        const Core2d::ImageFormat mapFormat = cubeFaces[0]->format();
        const Core2d::ImageDType mapDtype = cubeFaces[0]->dtype();

        const int glInternalFormat = Core2d::formatToGlInternalFormat(mapFormat);
        const int glFormat = Core2d::formatToGlFormat(mapFormat);
        const int glDtype = static_cast<int>(mapDtype);

        const GlCubemap envCubemap(cubeFaces, true);

        RenderToCubemap renderToCubemap(
            prefilteredCubeDim,
            glInternalFormat, glFormat, glDtype,
            true);

        std::string error;

        const Shaders::GlShader vertShader(
            "../data/shaders/prefilter_convolution.vert",
            Shaders::GlShaderType::VertexShader,
            error);
        Common::exitOnError(error, 1337);

        const Shaders::GlShader fragShader(
            "../data/shaders/prefilter_convolution.frag",
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

        for (int mipLevel = 0; mipLevel < prefilteredMipLevels; ++mipLevel) {
            renderToCubemap.useRenderbufferAtMipLevel(mipLevel);

            const float roughness = static_cast<float>(mipLevel) / (prefilteredMipLevels - 1);
            shaderProgram.setUniform("uRoughness", roughness);

            for (int faceInd = 0; faceInd < 6; ++faceInd) {
                shaderProgram.setUniform("uView", cubemapCaptureViews[faceInd]);
                renderToCubemap.useFramebufferTexture(faceInd, mipLevel);
                envCubemap.draw();
            }
        }

        const unsigned int prefilteredTextureId = renderToCubemap.textureId(true);
        return std::make_shared<GlCubemap>(prefilteredTextureId);
    }

    std::shared_ptr<GlTexture> integrateBrdf(const int lutTextureDim)
    {
        RenderToTexture renderToTexture(
            lutTextureDim, lutTextureDim,
            GL_RG32F, GL_RG, GL_FLOAT);

        std::string error;

        const Shaders::GlShader vertShader(
            "../data/shaders/brdf_integration.vert",
            Shaders::GlShaderType::VertexShader,
            error);
        Common::exitOnError(error, 1337);

        const Shaders::GlShader fragShader(
            "../data/shaders/brdf_integration.frag",
            Shaders::GlShaderType::FragmentShader,
            error);
        Common::exitOnError(error, 1338);

        const Shaders::GlShaderProgram shaderProgram(
            {vertShader, fragShader},
            error);
        Common::exitOnError(error, 1339);

        shaderProgram.use();

        renderToTexture.useFramebuffer();
        renderToTexture.useFramebufferTexture();

        const Meshes::GlQuad quad;
        quad.draw();

        const unsigned int brdfLutTextureId = renderToTexture.textureId(true);
        return std::make_shared<GlTexture>(brdfLutTextureId);
    }
}
