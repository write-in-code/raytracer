#pragma once
#include "Hittable.h"

class HittableList : public Hittable
{
public:
    HittableList() = default;
    HittableList(const HittablePtr &object) { Add(object); }

    void Clear() { m_objects.clear(); }

    void Add(const HittablePtr &object);

    bool Hit(const Ray &r, Interval rayT, HitRecord &rec) const override;

    std::vector<HittablePtr> m_objects;
};