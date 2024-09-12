#include "Pch.h"
#include "Material.h"

bool Lambertian::Scatter(const Ray &rIn, const HitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const
{
    glm::vec3 scatterDirection = rec.normal + RandomUnitVec();

    if (NearZero(scatterDirection))
        scatterDirection = rec.normal;

    scattered = Ray(rec.p, scatterDirection, rIn.Time());
    attenuation = m_albedo;
    return true;
}

bool Metal::Scatter(const Ray &rIn, const HitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const
{
    glm::vec3 reflected = glm::reflect(rIn.Direction(), rec.normal);
    reflected = glm::normalize(reflected) + (m_fuzz * RandomUnitVec());
    scattered = Ray(rec.p, reflected, rIn.Time());
    attenuation = m_albedo;
    return glm::dot(scattered.Direction(), rec.normal) > 0;
}

bool Dielectric::Scatter(const Ray &rIn, const HitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const
{
    attenuation = glm::vec3(1.f);
    float ri = rec.frontFace ? (1.f / m_refractionIndex) : m_refractionIndex;

    glm::vec3 unitDirection = glm::normalize(rIn.Direction());
    float cosTheta = glm::min(glm::dot(-unitDirection, rec.normal), 1.f);
    float sinTheta = glm::sqrt(1.f - cosTheta * cosTheta);

    auto reflectance = [](float cosine, float refractionIndex)
    {
        // Schlick's approximation
        float r0 = (1.f - refractionIndex) / (1.f + refractionIndex);
        r0 = r0 * r0;
        return r0 + (1.f - r0) * glm::pow(1.f - cosine, 5.f);
    };

    glm::vec3 direction = (ri * sinTheta > 1.f) || reflectance(cosTheta, m_refractionIndex) > RandomFloat() ? glm::reflect(unitDirection, rec.normal) : glm::refract(unitDirection, rec.normal, ri);

    scattered = Ray(rec.p, direction, rIn.Time());
    return true;
}