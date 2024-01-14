#include <vector>
#include "../../common/debug.h"
#include "../shaders/glshaderprogram.h"
#include "../meshes/glquad.h"
#include "gltexture.h"
#include "rendertotexture.h"
#include "material.h"

namespace Rendering::Textures {

    std::shared_ptr<Core2d::Image> heightMapToNormalMap(
        const std::shared_ptr<Meshes::GlMesh> &mesh,
        const Core2d::Image &heightMap,
        const float heightScale,
        const int nSmoothIters)
    {
        const int glDtype = static_cast<int>(heightMap.dtype());
        const int width = heightMap.width();
        const int height = heightMap.height();

        const GlTexture heightMapTexture(heightMap, false);

        RenderToTexture renderToTexture(
            width, height,
            GL_RGB, GL_RGB, glDtype);

        std::string error;

        const Shaders::GlShader vertShader(
            "../data/shaders/normal_map_generation.vert",
            Shaders::GlShaderType::VertexShader,
            error);
        Common::exitOnError(error, 1337);

        const Shaders::GlShader fragShader(
            "../data/shaders/normal_map_generation.frag",
            Shaders::GlShaderType::FragmentShader,
            error);
        Common::exitOnError(error, 1338);

        const Shaders::GlShaderProgram shaderProgram(
            {vertShader, fragShader},
            error);
        Common::exitOnError(error, 1339);

        shaderProgram.use();
        shaderProgram.setUniform("uHeightMap", 0);
        shaderProgram.setUniform("uHeightScale", heightScale);
        heightMapTexture.use(0);

        renderToTexture.useFramebuffer();
        renderToTexture.useFramebufferTexture();

        mesh->draw();

        auto normalMap = std::make_shared<Core2d::Image>(width, height, Core2d::ImageFormat::RGB);
        renderToTexture.readFramebuffer(normalMap->data());

        auto smoothNormalMap = normalMap;
        for (int smoothIter = 0; smoothIter < nSmoothIters; ++smoothIter)
            smoothNormalMap = gaussianBlur(*smoothNormalMap);

        return smoothNormalMap;
    }

    std::shared_ptr<Core2d::Image> gaussianBlur(const Core2d::Image &image)
    {
        const Core2d::ImageFormat imageFormat = image.format();
        const Core2d::ImageDType imageDtype = image.dtype();

        const int glInternalFormat = Core2d::formatToGlInternalFormat(imageFormat);
        const int glFormat = Core2d::formatToGlFormat(imageFormat);
        const int glDtype = static_cast<int>(imageDtype);

        const int imageWidth = image.width();
        const int imageHeight = image.height();

        const GlTexture imageTexture(image, false);

        RenderToTexture renderToTexture(
            imageWidth, imageHeight,
            glInternalFormat, glFormat, glDtype);

        std::string error;

        const Shaders::GlShader vertShader(
            "../data/shaders/image_blur.vert",
            Shaders::GlShaderType::VertexShader,
            error);
        Common::exitOnError(error, 1337);

        const Shaders::GlShader fragShader(
            "../data/shaders/image_blur.frag",
            Shaders::GlShaderType::FragmentShader,
            error);
        Common::exitOnError(error, 1338);

        const Shaders::GlShaderProgram shaderProgram(
            {vertShader, fragShader},
            error);
        Common::exitOnError(error, 1339);

        shaderProgram.use();
        shaderProgram.setUniform("uImage", 0);
        imageTexture.use(0);

        renderToTexture.useFramebuffer();
        renderToTexture.useFramebufferTexture();

        const Meshes::GlQuad quad;
        quad.draw();

        auto blurredImage = std::make_shared<Core2d::Image>(imageWidth, imageHeight, imageFormat);
        renderToTexture.readFramebuffer(blurredImage->data());
        return blurredImage;
    }
}
