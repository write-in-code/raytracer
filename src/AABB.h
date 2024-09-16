#pragma once
#include "Interval.h"
#include "Ray.h"

class AABB
{
public:
    AABB() = default;
    AABB(const Interval &x, const Interval &y, const Interval &z);
    AABB(const glm::vec3 &a, const glm::vec3 &b);
    AABB(const AABB &box0, const AABB &box1);

    const Interval &AxisInterval(int n) const;
    bool Hit(const Ray &r, Interval rayT) const;

    Interval x;
    Interval y;
    Interval z;
};