#include <sstream>
#include <vector>
#include "glshaderprogram.h"

namespace Rendering::Shaders {

    GlShaderProgram::GlShaderProgram(
        const std::vector<GlShader> &shaders,
        std::string &error)
    {
        error = "";

        m_programId = glCreateProgram();
        for (const GlShader &shader : shaders)
            glAttachShader(m_programId, shader.id());
        glLinkProgram(m_programId);

        int isSuccess;
        int logLength;
        std::vector<char> log;

        glGetProgramiv(m_programId, GL_LINK_STATUS, &isSuccess);
        if (!isSuccess) {
            glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &logLength);
            log.resize(logLength);
            glGetProgramInfoLog(m_programId, logLength, nullptr, log.data());
            glDeleteProgram(m_programId);
            error = std::string("Program link error. Log:\n") + log.data();
        }

        for (const GlShader &shader : shaders)
            glDetachShader(m_programId, shader.id());
    }

    GlShaderProgram::~GlShaderProgram()
    {
        glDeleteProgram(m_programId);
    }

    void GlShaderProgram::use() const
    {
        glUseProgram(m_programId);
    }

    int GlShaderProgram::uniformLocation(const std::string &name) const
    {
        return glGetUniformLocation(m_programId, name.c_str());
    }
}
