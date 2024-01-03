#ifndef CORE2D_IMAGE_H
#define CORE2D_IMAGE_H

#include <string>

namespace Core2d {

    enum class ImageFormat
    {
        Unchanged = 0,
        Grayscale = 1,
        GrayAlpha = 2,
        RGB = 3,
        RGBA = 4
    };

    int formatToNChannels(const ImageFormat format);
    int formatToGlValue(const ImageFormat format);
    ImageFormat formatFromNChannels(const int nChannels);

    class Image
    {
    public:
        explicit Image(
            const std::string &path,
            const ImageFormat desiredFormat = ImageFormat::Unchanged,
            const bool flipVertically = true);
        ~Image();

        const void *data() const;
        void *data();

        int width() const;
        int height() const;

        ImageFormat format() const;

    private:
        int m_width = 0;
        int m_height = 0;

        ImageFormat m_format;

        void *m_pixels;
    };
}

#endif
