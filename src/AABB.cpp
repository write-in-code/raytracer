#include "Pch.h"
#include "AABB.h"

const AABB AABB::Empty = AABB(Interval::Empty, Interval::Empty, Interval::Empty);
const AABB AABB::Universe = AABB(Interval::Universe, Interval::Universe, Interval::Universe);

AABB::AABB(const Interval &x, const Interval &y, const Interval &z)
    : x(x),
      y(y),
      z(z)
{
    PadToMinimums();
}

AABB::AABB(const glm::vec3 &a, const glm::vec3 &b)
{
    x = (a.x <= b.x) ? Interval(a.x, b.x) : Interval(b.x, a.x);
    y = (a.y <= b.y) ? Interval(a.y, b.y) : Interval(b.y, a.y);
    z = (a.z <= b.z) ? Interval(a.z, b.z) : Interval(b.z, a.z);

    PadToMinimums();
}

AABB::AABB(const AABB &box0, const AABB &box1)
{
    x = Interval(box0.x, box1.x);
    y = Interval(box0.y, box1.y);
    z = Interval(box0.z, box1.z);
}

const Interval &AABB::AxisInterval(int n) const
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

bool AABB::Hit(const Ray &r, Interval rayT) const
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

int AABB::LongestAxis() const
{
    if (x.Size() > y.Size())
    {
        return x.Size() > z.Size() ? 0 : 2;
    }
    else
    {
        return y.Size() > z.Size() ? 1 : 2;
    }
}

void AABB::PadToMinimums()
{
    constexpr float kDelta = 0.0001f;
    if (x.Size() < kDelta)
    {
        x = x.Expand(kDelta);
    }

    if (y.Size() < kDelta)
    {
        y = y.Expand(kDelta);
    }

    if (z.Size() < kDelta)
    {
        z = z.Expand(kDelta);
    }
}
