#ifndef RENDERING_TEXTURES_RENDERTOCUBEMAP_H
#define RENDERING_TEXTURES_RENDERTOCUBEMAP_H

namespace Rendering::Textures {

    class RenderToCubemap
    {
    public:
        explicit RenderToCubemap(
            const int cubeDim,
            const int glTextureInternalFormat,
            const int glTextureFormat,
            const int glTextureDtype);
        ~RenderToCubemap();

        void useFramebuffer() const;
        void useFramebufferTexture(const int faceInd) const;

        void readFramebuffer(void *outData) const;

    private:
        unsigned int m_framebufferObject = 0;
        unsigned int m_renderbufferObject = 0;
        unsigned int m_cubemapTextureId = 0;

        int m_cubemapDim = -1;
        int m_cubemapTextureFormat = -1;
        int m_cubemapTextureDtype = -1;
    };
}

#endif
