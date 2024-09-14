#pragma once
#include "Interval.h"
#include "Ray.h"

class AABB
{
public:
    Interval x;
    Interval y;
    Interval z;

    AABB() {}
    AABB(const Interval &x, const Interval &y, const Interval &z)
        : x(x),
          y(y),
          z(z)
    {
    }

    AABB(const glm::vec3 &a, const glm::vec3 &b)
    {
        x = (a.x <= b.x) ? Interval(a.x, b.x) : Interval(b.x, a.x);
        y = (a.y <= b.y) ? Interval(a.y, b.y) : Interval(b.y, a.y);
        z = (a.z <= b.z) ? Interval(a.z, b.z) : Interval(b.z, a.z);
    }

    AABB(const AABB &box0, const AABB &box1)
    {
        x = Interval(box0.x, box1.x);
        y = Interval(box0.y, box1.y);
        z = Interval(box0.z, box1.z);
    }

    const Interval &AxisInterval(int n) const
    {
        switch (n)
        {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        default:
            assert(false);
        }
    }

    bool Hit(const Ray &r, Interval rayT) const
    {
        const glm::vec3 &rayOrig = r.Origin();
        const glm::vec3 &rayDir = r.Direction();

        for (int axis = 0; axis < 3; ++axis)
        {
            const Interval &ax = AxisInterval(axis);
            const float adinv = 1.f / rayDir[axis];

            float t0 = (ax.min - rayOrig[axis]) * adinv;
            float t1 = (ax.max - rayOrig[axis]) * adinv;

            if (t0 < t1)
            {
                if (t0 > rayT.min)
                {
                    rayT.min = t0;
                }

                if (t1 < rayT.max)
                {
                    rayT.max = t1;
                }
            }
            else
            {
                if (t1 > rayT.min)
                {
                    rayT.min = t1;
                }

                if (t0 < rayT.max)
                {
                    rayT.max = t0;
                }
            }

            if (rayT.max <= rayT.min)
            {
                return false;
            }
        }

        return true;
    }
};