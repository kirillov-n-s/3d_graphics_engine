#include <fstream>
#include <sstream>
#include <vector>
#include "glshader.h"

namespace Rendering::Shaders {

    GlShader::GlShader(
        const std::string &codePath,
        const GlShaderType type,
        std::string &error)
    : m_shaderType(type)
    {
        error = "";

        std::ifstream codeFile;
        codeFile.open(codePath);
        if (!codeFile.is_open()) {
            error = "Cannot open file " + codePath;
            return;
        }

        std::ostringstream codeStream;
        codeStream << codeFile.rdbuf();
        codeFile.close();

        const std::string codeString = codeStream.str();
        const char *code = codeString.c_str();

        int isSuccess;
        int logLength;
        std::vector<char> log;

        const auto typeInt = static_cast<GLenum>(type);

        m_shaderId = glCreateShader(typeInt);
        glShaderSource(
                m_shaderId,
                1,
                &code,
                nullptr);
        glCompileShader(m_shaderId);
        glGetShaderiv(
            m_shaderId,
            GL_COMPILE_STATUS,
            &isSuccess);
        if (!isSuccess) {
            glGetShaderiv(
                m_shaderId,
                GL_INFO_LOG_LENGTH,
                &logLength);
            log.resize(logLength);
            glGetShaderInfoLog(
                m_shaderId,
                logLength,
                nullptr,
                log.data());
            glDeleteShader(m_shaderId);
            error = std::to_string(typeInt) + " shader compilation error. Log:\n" + log.data() + '\n';
        }
    }

    GlShaderType GlShader::type() const
    {
        return m_shaderType;
    }

    unsigned int GlShader::id() const
    {
        return m_shaderId;
    }

    GlShader::~GlShader()
    {
        glDeleteShader(m_shaderId);
    }
}
