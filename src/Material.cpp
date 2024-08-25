#include "Pch.h"
#include "Material.h"

bool Lambertian::Scatter(const Ray &, const HitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const
{
    glm::vec3 scatterDirection = rec.normal + RandomUnitVec();

    if (NearZero(scatterDirection))
        scatterDirection = rec.normal;

    scattered = Ray(rec.p, scatterDirection);
    attenuation = m_albedo;
    return true;
}

bool Metal::Scatter(const Ray &rIn, const HitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const
{
    glm::vec3 reflected = glm::reflect(rIn.Direction(), rec.normal);
    reflected = glm::normalize(reflected) + (m_fuzz * RandomUnitVec());
    scattered = Ray(rec.p, reflected);
    attenuation = m_albedo;
    return glm::dot(scattered.Direction(), rec.normal) > 0;
}