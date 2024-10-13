#include "RtwImage.h"

static Uint32 GetSurfacePixel(const SDL_SurfaceUPtr &surface, int x, int y)
{
    Uint8 bpp = surface->format->BytesPerPixel;
    Uint8 *p = static_cast<Uint8 *>(surface->pixels) + y * surface->pitch + x * bpp;

    switch (bpp)
    {
    case 1:
        return *p;
    case 2:
        return *reinterpret_cast<Uint16 *>(p);
    case 3:
        return SDL_BYTEORDER == SDL_BIG_ENDIAN ? (p[0] << 16 | p[1] << 8 | p[2]) : p[0] | p[1] << 8 | p[2] << 16;
    case 4:
        return *reinterpret_cast<Uint32 *>(p);
    default:
        assert(false);
    }

    return 0;
}

RtwImage::RtwImage(const char *imageFilename)
    : m_surface(IMG_Load(imageFilename))
{
    assert(m_surface);
}

glm::vec3 RtwImage::PixelData(int x, int y) const
{
    Uint32 pixel = GetSurfacePixel(m_surface, x, y);
    Uint8 r, g, b;
    SDL_GetRGB(pixel, m_surface->format, &r, &g, &b);

    return glm::vec3(r, g, b) / 255.f;
}