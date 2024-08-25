#pragma once
#include "Material.h"

class Sphere : public Hittable
{
public:
    Sphere(const glm::vec3 &center, float radius, const MaterialPtr &mat)
        : m_center(center),
          m_radius(glm::max(0.f, radius)),
          m_mat(mat)
    {
    }

    bool Hit(const Ray &r, Interval rayT, HitRecord &rec) const override;

private:
    glm::vec3 m_center;
    float m_radius;
    MaterialPtr m_mat;
};