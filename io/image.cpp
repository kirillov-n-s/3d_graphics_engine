#include <filesystem>
#include "stb_image/stb_image_write.h"
#include "image.h"

namespace IO {

    void writeImage(
        const Core2d::Image &image,
        const std::string &path,
        std::string &error,
        const ImageWriterSettings &settings)
    {
        error = "";

        const char *path_cstr = path.c_str();
        const int width = image.width();
        const int height = image.height();
        const int nChannels = Core2d::formatToNChannels(image.format());
        const void *data = image.data();

        const std::string ext = std::filesystem::path(path).extension().string();
        if (ext == ".png")
            stbi_write_png(
                path_cstr,
                width, height, nChannels,
                data,
                nChannels * width);
        else if (ext == ".jpg")
            stbi_write_jpg(
                path_cstr,
                width, height, nChannels,
                data,
                settings.jpgQuality);
        else if (ext == ".hdr")
            stbi_write_hdr(
                path_cstr,
                width, height, nChannels,
                static_cast<const float *>(data));
    }
}
