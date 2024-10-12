#pragma once

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