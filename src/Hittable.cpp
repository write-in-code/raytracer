#include "Pch.h"
#include "Hittable.h"

void HitRecord::SetFaceNormal(const Ray &r, const glm::vec3 &outwardNormal)
{
    frontFace = glm::dot(r.Direction(), outwardNormal) < 0.f;
    normal = frontFace ? outwardNormal : -outwardNormal;
}

bool Translate::Hit(const Ray &r, Interval rayT, HitRecord &rec) const
{
    Ray offsetR(r.Origin() - m_offset, r.Direction(), r.Time());

    if (!m_object->Hit(offsetR, rayT, rec))
    {
        return false;
    }

    rec.p += m_offset;
    return true;
}