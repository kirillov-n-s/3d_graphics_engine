#include <iostream>
#include <filesystem>
#include "../io/stb_image/stb_image.h"
#include "image.h"

namespace Core2d {

    void StbiImageDeleter::operator()(void *data) const
    {
        stbi_image_free(data);
    }

    void ManualImageDeleter::operator()(void *data) const
    {
        free(data);
    }

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
            case ImageFormat::HDR:
                return 3;
            default:
                std::cerr << "Incorrect image format" << std::endl;
                exit(228);
        }
    }

    int dtypeToNBytes(const ImageDType dtype)
    {
        switch (dtype) {
            case ImageDType::UnsignedByte:
                return 1;
            case ImageDType::Float:
                return 4;
            default:
                std::cerr << "Incorrect image dtype" << std::endl;
            exit(228);
        }
    }

    int formatToGlFormat(const ImageFormat format)
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
            case ImageFormat::HDR:
                return GL_RGB;
            default:
                std::cerr << "Incorrect image format" << std::endl;
                exit(228);
        }
    }

    int formatToGlInternalFormat(const ImageFormat format)
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
            case ImageFormat::HDR:
                return GL_RGB16F;
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
        int nActualChannels = 0;
        const std::string ext = std::filesystem::path(path).extension().string();
        if (ext == ".hdr") {
            m_pixels = stbi_loadf(
                path.c_str(),
                &m_width, &m_height, &nActualChannels,
                0);
            m_format = ImageFormat::HDR;
            m_dtype = ImageDType::Float;
        } else {
            const int nDesiredChannels = formatToNChannels(desiredFormat);
            m_pixels = stbi_load(
                path.c_str(),
                &m_width, &m_height, &nActualChannels,
                nDesiredChannels);
            const int nChannels = desiredFormat == ImageFormat::Unchanged ? nActualChannels : nDesiredChannels;
            m_format = formatFromNChannels(nChannels);
            m_dtype = ImageDType::UnsignedByte;
        }
        if (m_pixels == nullptr) {
            std::cerr << "Cannot read image " << path << std::endl;
            exit(229);
        }
        m_deleter = StbiImageDeleter();
    }

    Image::Image(
        const int width, const int height,
        const ImageFormat format)
    : m_width(width), m_height(height), m_format(format),
      m_dtype(format == ImageFormat::HDR ? ImageDType::Float : ImageDType::UnsignedByte),
      m_deleter(ManualImageDeleter())
    {
        const int nChannels = formatToNChannels(format);
        const int nBytesPerValue = dtypeToNBytes(m_dtype);
        m_pixels = malloc(width * height * nChannels * nBytesPerValue);
    }

    Image::~Image()
    {
        m_deleter(m_pixels);
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

    ImageDType Image::dtype() const
    {
        return m_dtype;
    }
}
