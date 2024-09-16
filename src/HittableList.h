#pragma once
#include "Hittable.h"

class HittableList : public Hittable
{
public:
    HittableList() = default;
    HittableList(const HittablePtr &object) { Add(object); }

    void Clear() { objects.clear(); }
    void Add(const HittablePtr &object);
    bool Hit(const Ray &r, Interval rayT, HitRecord &rec) const override;
    AABB BoundingBox() const override { return m_bbox; }

    std::vector<HittablePtr> objects;

private:
    AABB m_bbox;
};