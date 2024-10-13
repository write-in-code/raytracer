#pragma once
#include <SDL2/SDL_image.h>

class RtwImage
{
public:
    RtwImage() = default;
    RtwImage(const char *imageFilename);

    int Width() const { return m_surface->w; }
    int Height() const { return m_surface->h; }

    glm::vec3 PixelData(int x, int y) const;
    bool IsNull() const { return !m_surface; }

private:
    SDL_SurfaceUPtr m_surface;
};