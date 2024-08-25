#pragma once

using ImageData = std::unique_ptr<glm::u8vec4[]>;

struct ImageInfo
{
    ImageData data;
    int width;
    int height;
};

enum class OverwriteFileOption
{
    Overwrite,
    AddSuffix
};

SDL_SurfaceUPtr ImageInfoToSurface(const ImageInfo &imageInfo);

bool SavePPM(std::string_view filename, const ImageInfo &imageInfo, OverwriteFileOption opt);
bool SaveBMP(std::string_view filename, const ImageInfo &imageInfo, OverwriteFileOption opt);
