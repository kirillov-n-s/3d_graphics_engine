#ifndef IO_IMAGE_H
#define IO_IMAGE_H

#include <string>
#include "../core2d/image.h"

namespace IO {

    struct ImageWriterSettings
    {
        int jpgQuality = 100;
        bool flipVertically = true;
    };

    void writeImage(
        const Core2d::Image &image,
        const std::string &path,
        std::string &error,
        const ImageWriterSettings &settings = {});
}

#endif
