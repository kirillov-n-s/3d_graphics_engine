#include <vector>
#include <gl/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "../../common/debug.h"
#include "../../core3d/directions.h"
#include "../shaders/glshaderprogram.h"
#include "../meshes/glcube.h"
#include "gltexture.h"
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

        unsigned int framebufferObject, renderbufferObject;
        glGenFramebuffers(1, &framebufferObject);
        glGenRenderbuffers(1, &renderbufferObject);

        glBindFramebuffer(GL_FRAMEBUFFER, framebufferObject);
        glBindRenderbuffer(GL_RENDERBUFFER, renderbufferObject);
        glRenderbufferStorage(
            GL_RENDERBUFFER,
            GL_DEPTH_COMPONENT24,
            cubeDim, cubeDim);
        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_RENDERBUFFER,
            renderbufferObject);

        unsigned int envCubemap;
        glGenTextures(1, &envCubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
        for (unsigned int i = 0; i < 6; ++i)
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                glInternalFormat,
                cubeDim, cubeDim,
                0,
                glFormat,
                glDtype,
                nullptr);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glm::mat4 captureProjection = glm::perspective(
            glm::radians(90.0f),
            1.0f,
            0.1f, 10.0f);
        glm::mat4 captureViews[] = {
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
        shaderProgram.setUniform("uProjection", captureProjection);
        equirectangularTexture.use(0);

        glViewport(0, 0, cubeDim, cubeDim);
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferObject);

        std::array<std::shared_ptr<Core2d::Image>, 6> cubeFaces;
        for (int faceInd = 0; faceInd < 6; ++faceInd) {
            shaderProgram.setUniform("uView", captureViews[faceInd]);
            glFramebufferTexture2D(
                GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceInd,
                envCubemap,
                0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            cube.draw();

            cubeFaces[faceInd] = std::make_shared<Core2d::Image>(cubeDim, cubeDim, mapFormat);

            glGetTexImage(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceInd,
                0,
                Core2d::formatToGlFormat(mapFormat),
                static_cast<int>(mapDtype),
                cubeFaces[faceInd]->data());
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return cubeFaces;
    }

    std::array<std::shared_ptr<Core2d::Image>, 6> cubemapToIrradianceMap(
        const std::array<std::shared_ptr<Core2d::Image>, 6> &cubeFaces,
        const int irradianceCubeDim)
    {
        return cubeFaces;
    }
}
