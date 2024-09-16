#pragma once
#include "Material.h"
#include "AABB.h"

class Sphere : public Hittable
{
public:
    Sphere(const glm::vec3 &staticCenter, float radius, const MaterialPtr &mat);
    Sphere(const glm::vec3 &center1, const glm::vec3 &center2, float radius, const MaterialPtr &mat);

    bool Hit(const Ray &r, Interval rayT, HitRecord &rec) const override;
    AABB BoundingBox() const override { return m_bbox; }

private:
    Ray m_center;
    float m_radius;
    MaterialPtr m_mat;
    AABB m_bbox;
};