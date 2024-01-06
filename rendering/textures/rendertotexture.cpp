#include <gl/glew.h>
#include "rendertotexture.h"

namespace Rendering::Textures {

    RenderToTexture::RenderToTexture(
        const int width, const int height,
        const int glTextureInternalFormat,
        const int glTextureFormat,
        const int glTextureDtype)
    : m_textureWidth(width), m_textureHeight(height),
      m_textureFormat(glTextureFormat), m_textureDtype(glTextureDtype)
    {
        glGenFramebuffers(1, &m_framebufferObject);
        glGenRenderbuffers(1, &m_renderbufferObject);
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferObject);
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderbufferObject);
        glRenderbufferStorage(
            GL_RENDERBUFFER,
            GL_DEPTH_COMPONENT24,
            width, height);
        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_RENDERBUFFER,
            m_renderbufferObject);

        glGenTextures(1, &m_textureId);
        glBindTexture(GL_TEXTURE_2D, m_textureId);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            glTextureInternalFormat,
            width, height,
            0,
            glTextureFormat,
            glTextureDtype,
            nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    RenderToTexture::~RenderToTexture()
    {
        glDeleteFramebuffers(1, &m_framebufferObject);
        glDeleteRenderbuffers(1, &m_renderbufferObject);
        if (m_ownsTexture)
            glDeleteTextures(1, &m_textureId);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void RenderToTexture::useFramebuffer() const
    {
        glViewport(0, 0, m_textureWidth, m_textureHeight);
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferObject);
    }

    void RenderToTexture::useFramebufferTexture() const
    {
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D,
            m_textureId,
            0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RenderToTexture::readFramebuffer(void *outData) const
    {
        glReadPixels(
            0, 0, m_textureWidth, m_textureHeight,
            m_textureFormat,
            m_textureDtype,
            outData);
    }

    unsigned RenderToTexture::textureId(const bool transferTextureOwnership)
    {
        if (transferTextureOwnership)
            m_ownsTexture = false;
        return m_textureId;
    }

}
