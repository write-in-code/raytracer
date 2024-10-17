#pragma once
#include "Hittable.h"

class Quad : public Hittable
{
public:
    Quad(const glm::vec3 &q, const glm::vec3 &u, const glm::vec3 &v, const MaterialPtr &mat);

    virtual void SetBoundingBox();

    AABB BoundingBox() const override { return m_bbox; }
    bool Hit(const Ray &, Interval, HitRecord &) const override { return false; }

private:
    glm::vec3 m_q;
    glm::vec3 m_u;
    glm::vec3 m_v;
    MaterialPtr m_mat;
    AABB m_bbox;
};