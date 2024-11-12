#include "Pch.h"
#include "ConstantMedium.h"

bool ConstantMedium::Hit(const Ray &r, Interval rayT, HitRecord &rec) const
{
    HitRecord rec1, rec2;
    if (!m_boundary->Hit(r, Interval::Universe, rec1))
    {
        return false;
    }

    if (!m_boundary->Hit(r, Interval(rec1.t + 0.0001f, std::numeric_limits<float>::infinity()), rec2))
    {
        return false;
    }

    if (rec1.t < rayT.min)
    {
        rec1.t = rayT.min;
    }

    if (rec2.t > rayT.max)
    {
        rec2.t = rayT.max;
    }

    if (rec1.t >= rec2.t)
    {
        return false;
    }

    if (rec1.t < 0.f)
    {
        rec1.t = 0.f;
    }

    float rayLength = r.Direction().length();
    float distanceInsideBoundary = (rec2.t - rec1.t) * rayLength;
    float hitDistance = m_negInvDensity * glm::log(RandomFloat());

    if (hitDistance > distanceInsideBoundary)
    {
        return false;
    }

    rec.t = rec1.t + hitDistance / rayLength;
    rec.p = r.At(rec.t);

    rec.normal = glm::vec3(1.f, 0.f, 0.f);
    rec.frontFace = true;
    rec.mat = m_phaseFunction;

    return true;
}