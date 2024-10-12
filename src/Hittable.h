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

    void setFaceNormal(const Ray &r, const glm::vec3 &outwardNormal)
    {
        frontFace = glm::dot(r.Direction(), outwardNormal) < 0.f;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

DEFINE_CLASS_PTR(Hittable);

class Hittable
{
public:
    virtual ~Hittable() = default;

    virtual bool Hit(const Ray &r, Interval rayT, HitRecord &rec) const = 0;
    virtual AABB BoundingBox() const = 0;
};