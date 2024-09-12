#pragma once
#include "Material.h"

class Sphere : public Hittable
{
public:
    Sphere(const glm::vec3 &staticCenter, float radius, const MaterialPtr &mat)
        : m_center(staticCenter, glm::vec3(0.f)),
          m_radius(glm::max(0.f, radius)),
          m_mat(mat)
    {
    }

    Sphere(const glm::vec3 &center1, const glm::vec3 &center2, float radius, const MaterialPtr &mat)
        : m_center(center1, (center2 - center1)),
          m_radius(glm::max(0.f, radius)),
          m_mat(mat)
    {
    }

    bool Hit(const Ray &r, Interval rayT, HitRecord &rec) const override;

private:
    Ray m_center;
    float m_radius;
    MaterialPtr m_mat;
};