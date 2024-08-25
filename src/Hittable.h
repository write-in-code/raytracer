#pragma once
#include "Interval.h"
#include "Ray.h"

DEFINE_CLASS_PTR(Material);

struct HitRecord
{
    glm::vec3 p;
    glm::vec3 normal;
    MaterialPtr mat;
    float t;
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
};