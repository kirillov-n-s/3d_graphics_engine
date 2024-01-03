#ifndef RENDERING_SHADERS_GLSHADER_H
#define RENDERING_SHADERS_GLSHADER_H

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Rendering::Shaders {

    enum class GlShaderType
    {
        VertexShader = GL_VERTEX_SHADER,
        FragmentShader = GL_FRAGMENT_SHADER,
        GeometryShader = GL_GEOMETRY_SHADER,
        TessControlShader = GL_TESS_CONTROL_SHADER,
        TessEvalShader = GL_TESS_EVALUATION_SHADER,
        ComputeShader = GL_COMPUTE_SHADER
    };

    class GlShader
    {
    public:
        GlShader(
            const std::string &codePath,
            const GlShaderType type,
            std::string &error);
        ~GlShader();

        GlShaderType type() const;

        unsigned int id() const;

    private:
        GlShaderType m_shaderType;
        unsigned int m_shaderId = 0;
    };
}

#endif
