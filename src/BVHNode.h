#pragma once
#include "AABB.h"
#include "HittableList.h"

class BVHNode : public Hittable
{
public:
    BVHNode(HittableList list)
        : BVHNode(list.objects, 0, list.objects.size()) {}
    BVHNode(std::vector<HittablePtr> &objects, size_t start, size_t end);

    bool Hit(const Ray &r, Interval rayT, HitRecord &rec) const override;
    AABB BoundingBox() const override { return m_bbox; }

private:
    static bool BoxCompare(const HittablePtr &a, const HittablePtr &b, int axisIndex);
    static bool BoxXCompare(const HittablePtr &a, const HittablePtr &b) { return BoxCompare(a, b, 0); }
    static bool BoxYCompare(const HittablePtr &a, const HittablePtr &b) { return BoxCompare(a, b, 1); }
    static bool BoxZCompare(const HittablePtr &a, const HittablePtr &b) { return BoxCompare(a, b, 2); }

    HittablePtr m_left;
    HittablePtr m_right;
    AABB m_bbox;
};