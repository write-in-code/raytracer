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
    int LongestAxis() const;

    Interval x;
    Interval y;
    Interval z;

    static const AABB Empty, Universe;

private:
    void PadToMinimums();
};

inline AABB operator+(const AABB &box, const glm::vec3 &offset)
{
    return AABB(box.x + offset.x, box.y + offset.y, box.z + offset.z);
}

inline AABB operator+(const glm::vec3 &offset, const AABB &box)
{
    return box + offset;
}