#ifndef RENDERING_SHADERS_GLSHADERPROGRAM_H
#define RENDERING_SHADERS_GLSHADERPROGRAM_H

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glshader.h"

namespace Rendering::Shaders {

    class GlShaderProgram
    {
    public:
        GlShaderProgram(
            const std::vector<GlShader> &shaders,
            std::string &error);
        ~GlShaderProgram();

        void use() const;

        template <typename UniformType>
        void setUniform(
            const std::string& name,
            const UniformType &value) const;

    private:
        unsigned int m_programId;

        int uniformLocation(const std::string& name) const;
    };

    template <typename>
    constexpr bool is_always_false = false;

    template<typename UniformType>
    void GlShaderProgram::setUniform(
        const std::string &name,
        const UniformType &value) const
    {
        const int location = uniformLocation(name);
        if constexpr(std::is_same_v<UniformType, float>)
            glUniform1f(location, value);
        else if constexpr(std::is_same_v<UniformType, glm::vec3>)
            glUniform3f(location, value.x, value.y, value.z);
        else if constexpr(std::is_same_v<UniformType, glm::vec4>)
            glUniform4f(location, value.x, value.y, value.z, value.w);
        else if constexpr(std::is_same_v<UniformType, glm::mat4>)
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
        else if constexpr(std::is_same_v<UniformType, int>)
            glUniform1i(location, value);
        else
            static_assert(is_always_false<UniformType>, "Uniform of that type is not supported.");
    }
}

#endif
