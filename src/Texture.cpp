#include "Pch.h"
#include "Texture.h"

glm::vec3 CheckerTexture::Value(float u, float v, const glm::vec3 &p) const
{
    auto xInteger = int(std::floor(m_invScale * p.x));
    auto yInteger = int(std::floor(m_invScale * p.y));
    auto zInteger = int(std::floor(m_invScale * p.z));
    bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;
    return isEven ? m_even->Value(u, v, p) : m_odd->Value(u, v, p);
}

glm::vec3 ImageTexture::Value(float u, float v, const glm::vec3 &) const
{
    if (m_image.IsNull())
    {
        return glm::vec3(0.f, 1.f, 1.f);
    }

    u = Interval(0.f, 1.f).Clamp(u);
    v = 1.f - Interval(0.f, 1.f).Clamp(v);

    auto i = int(u * m_image.Width());
    auto j = int(v * m_image.Height());
    return m_image.PixelData(i, j);
}