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