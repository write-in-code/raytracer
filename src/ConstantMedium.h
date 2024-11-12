#pragma once

#include "Hittable.h"
#include "Material.h"
#include "Texture.h"

class ConstantMedium : public Hittable
{
public:
    ConstantMedium(const HittablePtr &boundary, float density, const TexturePtr &tex)
        : m_boundary(boundary),
          m_negInvDensity(-1.f / density),
          m_phaseFunction(std::make_shared<Isotropic>(tex))
    {
    }

    ConstantMedium(const HittablePtr &boundary, float density, const glm::vec3 &albedo)
        : m_boundary(boundary),
          m_negInvDensity(-1.f / density),
          m_phaseFunction(std::make_shared<Isotropic>(albedo))
    {
    }

    bool Hit(const Ray &r, Interval rayT, HitRecord &rec) const override;
    AABB BoundingBox() const override { return m_boundary->BoundingBox(); }

private:
    HittablePtr m_boundary;
    float m_negInvDensity;
    MaterialPtr m_phaseFunction;
};
