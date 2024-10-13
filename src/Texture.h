#pragma once
#include "RtwImage.h"
#include "Interval.h"

DEFINE_CLASS_PTR(Texture);

class Texture
{
public:
    virtual ~Texture() = default;
    virtual glm::vec3 Value(float u, float v, const glm::vec3 &p) const = 0;
};

class SolidColor : public Texture
{
public:
    SolidColor(const glm::vec3 &albedo)
        : m_albedo(albedo) {}

    SolidColor(float red, float green, float blue)
        : SolidColor(glm::vec3(red, green, blue)) {}

    glm::vec3 Value(float, float, const glm::vec3 &) const override
    {
        return m_albedo;
    }

private:
    glm::vec3 m_albedo;
};

class CheckerTexture : public Texture
{
public:
    CheckerTexture(float scale, const TexturePtr &even, const TexturePtr &odd)
        : m_invScale(1.f / scale),
          m_even(even),
          m_odd(odd)
    {
    }

    CheckerTexture(float scale, const glm::vec3 &c1, const glm::vec3 &c2)
        : CheckerTexture(scale, std::make_shared<SolidColor>(c1), std::make_shared<SolidColor>(c2))
    {
    }

    glm::vec3 Value(float u, float v, const glm::vec3 &p) const override;

private:
    float m_invScale;
    TexturePtr m_even;
    TexturePtr m_odd;
};

class ImageTexture : public Texture
{
public:
    ImageTexture(const char *imageFilename)
        : m_image(imageFilename) {}

    glm::vec3 Value(float u, float v, const glm::vec3 &) const override
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

private:
    RtwImage m_image;
};