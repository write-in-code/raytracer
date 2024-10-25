#pragma once
#include "Interval.h"
#include "Ray.h"
#include "AABB.h"

DEFINE_CLASS_PTR(Material);

struct HitRecord
{
    glm::vec3 p;
    glm::vec3 normal;
    MaterialPtr mat;
    float t;
    float u;
    float v;
    bool frontFace;

    void SetFaceNormal(const Ray &r, const glm::vec3 &outwardNormal);
};

DEFINE_CLASS_PTR(Hittable);

class Hittable
{
public:
    virtual ~Hittable() = default;

    virtual bool Hit(const Ray &r, Interval rayT, HitRecord &rec) const = 0;
    virtual AABB BoundingBox() const = 0;
};

class Translate : public Hittable
{
public:
    Translate(const HittablePtr &object, const glm::vec3 &offset)
        : m_object(object),
          m_offset(offset)
    {
        m_bbox = m_object->BoundingBox() + m_offset;
    }

    bool Hit(const Ray &r, Interval rayT, HitRecord &rec) const override;
    AABB BoundingBox() const override { return m_bbox; }

private:
    HittablePtr m_object;
    glm::vec3 m_offset;
    AABB m_bbox;
};