#include <gl/glew.h>
#include "rendertocubemap.h"

namespace Rendering::Textures {

    RenderToCubemap::RenderToCubemap(
        const int cubeDim,
        const int glTextureInternalFormat,
        const int glTextureFormat,
        const int glTextureDtype)
    : m_cubemapDim(cubeDim),
      m_cubemapTextureFormat(glTextureFormat),
      m_cubemapTextureDtype(glTextureDtype)
    {
        glGenFramebuffers(1, &m_framebufferObject);
        glGenRenderbuffers(1, &m_renderbufferObject);
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferObject);
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderbufferObject);
        glRenderbufferStorage(
            GL_RENDERBUFFER,
            GL_DEPTH_COMPONENT24,
            cubeDim, cubeDim);
        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_RENDERBUFFER,
            m_renderbufferObject);

        glGenTextures(1, &m_cubemapTextureId);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapTextureId);
        for (int faceInd = 0; faceInd < 6; ++faceInd)
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceInd,
                0,
                glTextureInternalFormat,
                cubeDim, cubeDim,
                0,
                glTextureFormat,
                glTextureDtype,
                nullptr);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    RenderToCubemap::~RenderToCubemap()
    {
        glDeleteFramebuffers(1, &m_framebufferObject);
        glDeleteRenderbuffers(1, &m_renderbufferObject);
        glDeleteTextures(1, &m_cubemapTextureId);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void RenderToCubemap::useFramebuffer() const
    {
        glViewport(0, 0, m_cubemapDim, m_cubemapDim);
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferObject);
    }

    void RenderToCubemap::useFramebufferTexture(const int faceInd) const
    {
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceInd,
            m_cubemapTextureId,
            0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RenderToCubemap::readFramebuffer(void *outData) const
    {
        glReadPixels(
            0, 0, m_cubemapDim, m_cubemapDim,
            m_cubemapTextureFormat,
            m_cubemapTextureDtype,
            outData);
    }

}
