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

RotateY::RotateY(const HittablePtr &object, float angle)
    : m_object(object)
{
    float radians = glm::radians(angle);
    m_sinTheta = glm::sin(radians);
    m_cosTheta = glm::cos(radians);
    m_bbox = m_object->BoundingBox();

    glm::vec3 min(std::numeric_limits<float>::infinity());
    glm::vec3 max(-std::numeric_limits<float>::infinity());

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            for (int k = 0; k < 2; ++k)
            {
                float x = i * m_bbox.x.max + (1 - i) * m_bbox.x.min;
                float y = j * m_bbox.y.max + (1 - j) * m_bbox.y.min;
                float z = k * m_bbox.z.max + (1 - k) * m_bbox.z.min;

                float newx = m_cosTheta * x + m_sinTheta * z;
                float newz = -m_sinTheta * x + m_cosTheta * z;

                glm::vec3 tester(newx, y, newz);

                for (int c = 0; c < 3; ++c)
                {
                    min[c] = glm::min(min[c], tester[c]);
                    max[c] = glm::max(max[c], tester[c]);
                }
            }
        }
    }

    m_bbox = AABB(min, max);
}

bool RotateY::Hit(const Ray &r, Interval rayT, HitRecord &rec) const
{
    glm::vec3 origin = glm::vec3(
        m_cosTheta * r.Origin().x - m_sinTheta * r.Origin().z,
        r.Origin().y,
        m_sinTheta * r.Origin().x + m_cosTheta * r.Origin().z);

    glm::vec3 direction = glm::vec3(
        m_cosTheta * r.Direction().x - m_sinTheta * r.Direction().z,
        r.Direction().y,
        m_sinTheta * r.Direction().x + m_cosTheta * r.Direction().z);

    Ray rotatedR(origin, direction, r.Time());

    if (!m_object->Hit(rotatedR, rayT, rec))
    {
        return false;
    }

    rec.p = glm::vec3(
        m_cosTheta * rec.p.x + m_sinTheta * rec.p.z,
        rec.p.y,
        -m_sinTheta * rec.p.x + m_cosTheta * rec.p.z);

    rec.normal = glm::vec3(
        m_cosTheta * rec.normal.x + m_sinTheta * rec.normal.z,
        rec.normal.y,
        -m_sinTheta * rec.normal.x + m_cosTheta * rec.normal.z);

    return true;
}