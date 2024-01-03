#include <iostream>
#include <gl/glew.h>
#include "../io/stb_image/stb_image.h"
#include "image.h"

namespace Core2d {

    int formatToNChannels(const ImageFormat format)
    {
        switch (format) {
            case ImageFormat::Unchanged:
                return 0;
            case ImageFormat::Grayscale:
                return 1;
            case ImageFormat::GrayAlpha:
                return 2;
            case ImageFormat::RGB:
                return 3;
            case ImageFormat::RGBA:
                return 4;
            default:
                std::cerr << "Incorrect image format" << std::endl;
                exit(228);
        }
    }

    int formatToGlValue(const ImageFormat format)
    {
        switch (format) {
            case ImageFormat::Grayscale:
                return GL_RED;
            case ImageFormat::GrayAlpha:
                return GL_RG;
            case ImageFormat::RGB:
                return GL_RGB;
            case ImageFormat::RGBA:
                return GL_RGBA;
            default:
                std::cerr << "Incorrect image format" << std::endl;
                exit(228);
        }
    }

    ImageFormat formatFromNChannels(const int nChannels)
    {
        switch (nChannels) {
            case 0:
                return ImageFormat::Unchanged;
            case 1:
                return ImageFormat::Grayscale;
            case 2:
                return ImageFormat::GrayAlpha;
            case 3:
                return ImageFormat::RGB;
            case 4:
                return ImageFormat::RGBA;
            default:
                std::cerr << "Incorrect n channels" << std::endl;
            exit(228);
        }
    }

    Image::Image(
        const std::string &path,
        const ImageFormat desiredFormat,
        const bool flipVertically)
    {
        stbi_set_flip_vertically_on_load(flipVertically);
        const int nDesiredChannels = formatToNChannels(desiredFormat);
        int nActualChannels = 0;
        m_pixels = stbi_load(
            path.c_str(),
            &m_width, &m_height, &nActualChannels,
            nDesiredChannels);
        const int nChannels =
            desiredFormat == ImageFormat::Unchanged ? nActualChannels : nDesiredChannels;
        m_format = formatFromNChannels(nChannels);
    }

    Image::~Image()
    {
        stbi_image_free(m_pixels);
    }

    const void *Image::data() const
    {
        return m_pixels;
    }

    void *Image::data()
    {
        return m_pixels;
    }

    int Image::width() const
    {
        return m_width;
    }

    int Image::height() const
    {
        return m_height;
    }

    ImageFormat Image::format() const
    {
        return m_format;
    }
}
