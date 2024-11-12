#pragma once
#include "Ray.h"
#include "Hittable.h"
#include "Texture.h"

DEFINE_CLASS_PTR(Material);

class Material
{
public:
    virtual ~Material() = default;

    virtual bool Scatter(const Ray &, const HitRecord &, glm::vec3 &, Ray &) const { return false; }
    virtual glm::vec3 Emitted(float, float, const glm::vec3 &) const { return glm::vec3(0.f); }
};

class Lambertian : public Material
{
public:
    Lambertian(const glm::vec3 &albedo)
        : m_tex(std::make_shared<SolidColor>(albedo)) {}

    Lambertian(const TexturePtr &tex)
        : m_tex(tex) {}

    bool Scatter(const Ray &, const HitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const override;

private:
    TexturePtr m_tex;
};

class Metal : public Material
{
public:
    Metal(const glm::vec3 &albedo, float fuzz)
        : m_albedo(albedo), m_fuzz(glm::min(fuzz, 1.f)) {}

    bool Scatter(const Ray &rIn, const HitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const override;

private:
    glm::vec3 m_albedo;
    float m_fuzz;
};

class Dielectric : public Material
{
public:
    Dielectric(float refractionIndex)
        : m_refractionIndex(refractionIndex) {}

    bool Scatter(const Ray &rIn, const HitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const override;

private:
    float m_refractionIndex;
};

class DiffuseLight : public Material
{
public:
    DiffuseLight(const TexturePtr &tex)
        : m_tex(tex)
    {
    }

    DiffuseLight(const glm::vec3 &emit)
        : m_tex(std::make_shared<SolidColor>(emit))
    {
    }

    glm::vec3 Emitted(float u, float v, const glm::vec3 &p) const override { return m_tex->Value(u, v, p); }

private:
    TexturePtr m_tex;
};

class Isotropic : public Material
{
public:
    Isotropic(const glm::vec3 &albedo)
        : m_tex(std::make_shared<SolidColor>(albedo)) {}
    Isotropic(const TexturePtr &tex)
        : m_tex(tex) {}

    bool Scatter(const Ray &rIn, const HitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const override;

private:
    TexturePtr m_tex;
};