#pragma once
#include "Material.h"
#include "AABB.h"

class Sphere : public Hittable
{
public:
    Sphere(const glm::vec3 &staticCenter, float radius, const MaterialPtr &mat)
        : m_center(staticCenter, glm::vec3(0.f)),
          m_radius(glm::max(0.f, radius)),
          m_mat(mat)
    {
        glm::vec3 rvec = glm::vec3(radius);
        m_bbox = AABB(staticCenter - rvec, staticCenter + rvec);
    }

    Sphere(const glm::vec3 &center1, const glm::vec3 &center2, float radius, const MaterialPtr &mat)
        : m_center(center1, (center2 - center1)),
          m_radius(glm::max(0.f, radius)),
          m_mat(mat)
    {
        glm::vec3 rvec = glm::vec3(radius);
        AABB box1(m_center.At(0) - rvec, m_center.At(0) + rvec);
        AABB box2(m_center.At(1) - rvec, m_center.At(1) + rvec);
        m_bbox = AABB(box1, box2);
    }

    bool Hit(const Ray &r, Interval rayT, HitRecord &rec) const override;
    AABB BoundingBox() const override { return m_bbox; }

private:
    Ray m_center;
    float m_radius;
    MaterialPtr m_mat;
    AABB m_bbox;
};