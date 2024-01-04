#ifndef CORE2D_IMAGE_H
#define CORE2D_IMAGE_H

#include <string>
#include <functional>
#include <gl/glew.h>

namespace Core2d {

    using ImageDataDeleter = std::function<void(void *)>;

    struct StbiImageDeleter
    {
        void operator()(void *data) const;
    };

    struct ManualImageDeleter
    {
        void operator()(void *data) const;
    };

    enum class ImageFormat
    {
        Unchanged = 0,
        Grayscale = 1,
        GrayAlpha = 2,
        RGB = 3,
        RGBA = 4,
        HDR = 5,
    };

    enum class ImageDType
    {
        UnsignedByte = GL_UNSIGNED_BYTE,
        Float = GL_FLOAT
    };

    int formatToNChannels(const ImageFormat format);
    int dtypeToNBytes(const ImageDType dtype);
    int formatToGlFormat(const ImageFormat format);
    int formatToGlInternalFormat(const ImageFormat format);
    ImageFormat formatFromNChannels(const int nChannels);

    class Image
    {
    public:
        explicit Image(
            const std::string &path,
            const ImageFormat desiredFormat = ImageFormat::Unchanged,
            const bool flipVertically = true);
        Image(
            const int width, const int height,
            const ImageFormat format);
        ~Image();

        const void *data() const;
        void *data();

        int width() const;
        int height() const;

        ImageFormat format() const;
        ImageDType dtype() const;

    private:
        int m_width = 0;
        int m_height = 0;

        ImageFormat m_format;
        ImageDType m_dtype;

        void *m_pixels = nullptr;

        ImageDataDeleter m_deleter = nullptr;
    };
}

#endif
