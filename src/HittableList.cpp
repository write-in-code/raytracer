#include "Pch.h"
#include "HittableList.h"

void HittableList::Add(const HittablePtr &object)
{
    m_objects.push_back(object);
}

bool HittableList::Hit(const Ray &r, Interval rayT, HitRecord &rec) const
{
    HitRecord tempRec;
    bool hitAnything = false;
    float closestSoFar = rayT.max;

    for (const HittablePtr &object : m_objects)
    {
        if (object->Hit(r, Interval(rayT.min, closestSoFar), tempRec))
        {
            hitAnything = true;
            closestSoFar = tempRec.t;
            rec = tempRec;
        }
    }

    return hitAnything;
}