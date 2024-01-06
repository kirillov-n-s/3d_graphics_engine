#ifndef RENDERING_TEXTURES_RENDERTOTEXTURE_H
#define RENDERING_TEXTURES_RENDERTOTEXTURE_H

namespace Rendering::Textures {

    class RenderToTexture
    {
    public:
        explicit RenderToTexture(
            const int width, const int height,
            const int glTextureInternalFormat,
            const int glTextureFormat,
            const int glTextureDtype);
        ~RenderToTexture();

        void useFramebuffer() const;
        void useFramebufferTexture() const;

        void readFramebuffer(void *outData) const;

        unsigned int textureId(const bool transferTextureOwnership);
    private:
        unsigned int m_framebufferObject = 0;
        unsigned int m_renderbufferObject = 0;
        unsigned int m_textureId = 0;

        int m_textureWidth = -1;
        int m_textureHeight = -1;
        int m_textureFormat = -1;
        int m_textureDtype = -1;

        bool m_ownsTexture = true;
    };
}

#endif
