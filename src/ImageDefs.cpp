#include "Pch.h"
#include "ImageDefs.h"

using FilePtr = std::unique_ptr<FILE, DeleterFunc<fclose>>;

static std::optional<std::string> PickFilename(std::string_view filename)
{
    auto pos = filename.find_last_of('.');
    std::string name, ext;
    if (pos != std::string_view::npos)
    {
        name = filename.substr(0, pos);
        ext = filename.substr(pos);
    }
    else
    {
        name = filename;
    }

    int count = 0;
    constexpr int maxCount = 1000;

    std::string outName;
    do
    {
        outName = std::format("{}{:03}{}", name, ++count, ext);
    } while (std::filesystem::exists(outName) && count < maxCount);

    if (count < maxCount)
    {
        return outName;
    }

    return std::nullopt;
}

SDL_SurfaceUPtr ImageInfoToSurface(const ImageInfo &imageInfo)
{
    return SDL_SurfaceUPtr(SDL_CreateRGBSurfaceWithFormatFrom(imageInfo.data.get(), imageInfo.width, imageInfo.height, 32, imageInfo.width * 4, SDL_PIXELFORMAT_RGBA32));
}

bool SavePPM(std::string_view filename, const ImageInfo &imageInfo, OverwriteFileOption opt)
{
    std::string pickedFilename(filename);
    if (opt == OverwriteFileOption::AddSuffix && std::filesystem::exists(filename))
    {
        if (auto pickedFilenameOpt = PickFilename(filename))
        {
            pickedFilename = *pickedFilenameOpt;
        }
    }

    auto file = FilePtr(fopen(pickedFilename.c_str(), "w"));
    if (file)
    {
        fprintf(file.get(), "P3\n%d %d\n255\n", imageInfo.width, imageInfo.height);
        for (int j = 0; j < imageInfo.height; ++j)
        {
            for (int i = 0; i < imageInfo.width; ++i)
            {
                const glm::u8vec4 &p = imageInfo.data[j * imageInfo.width + i];
                fprintf(file.get(), "%" PRIu8 " %" PRIu8 " %" PRIu8 "\n", p.r, p.g, p.b);
            }
        }

        return true;
    }

    return false;
}

bool SaveBMP(std::string_view filename, const ImageInfo &imageInfo, OverwriteFileOption opt)
{
    std::string pickedFilename(filename);
    if (opt == OverwriteFileOption::AddSuffix && std::filesystem::exists(filename))
    {
        if (auto pickedFilenameOpt = PickFilename(filename))
        {
            pickedFilename = *pickedFilenameOpt;
        }
    }

    SDL_SurfaceUPtr surface = ImageInfoToSurface(imageInfo);
    if (surface)
    {
        SDL_SaveBMP(surface.get(), pickedFilename.c_str());
        return true;
    }

    return false;
}
