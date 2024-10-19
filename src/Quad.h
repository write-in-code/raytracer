#pragma once
#include "Hittable.h"

class Quad : public Hittable
{
public:
    Quad(const glm::vec3 &Q, const glm::vec3 &u, const glm::vec3 &v, const MaterialPtr &mat);

    virtual void SetBoundingBox();

    AABB BoundingBox() const override { return m_bbox; }
    bool Hit(const Ray &r, Interval rayT, HitRecord &rec) const override;
    virtual bool IsInterior(float a, float b, HitRecord &rec) const;

private:
    glm::vec3 m_Q;
    glm::vec3 m_u;
    glm::vec3 m_v;
    glm::vec3 m_w;
    MaterialPtr m_mat;
    AABB m_bbox;
    glm::vec3 m_normal;
    float m_D;
};