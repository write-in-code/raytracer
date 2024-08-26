#pragma once
#include "Ray.h"
#include "Hittable.h"

DEFINE_CLASS_PTR(Material);

class Material
{
public:
    virtual ~Material() = default;

    virtual bool Scatter(const Ray &, const HitRecord &, glm::vec3 &, Ray &) const { return false; }
};

class Lambertian : public Material
{
public:
    Lambertian(const glm::vec3 &albedo)
        : m_albedo(albedo) {}

    bool Scatter(const Ray &, const HitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const override;

private:
    glm::vec3 m_albedo;
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
    Dielectric(float refractionIndex) : m_refractionIndex(refractionIndex) {}

    bool Scatter(const Ray &rIn, const HitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const override;

private:
    float m_refractionIndex;
};