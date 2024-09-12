#include "Pch.h"
#include "Sphere.h"

bool Sphere::Hit(const Ray &r, Interval rayT, HitRecord &rec) const
{
    glm::vec3 currentCenter = m_center.At(r.Time());
    glm::vec3 oc = currentCenter - r.Origin();
    float a = glm::dot(r.Direction(), r.Direction());
    float h = glm::dot(r.Direction(), oc);
    float c = glm::dot(oc, oc) - m_radius * m_radius;

    float discriminant = h * h - a * c;
    if (discriminant < 0.f)
    {
        return false;
    }

    float sqrtd = glm::sqrt(discriminant);

    float root = (h - sqrtd) / a;
    if (!rayT.Surrounds(root))
    {
        root = (h + sqrtd) / a;
        if (!rayT.Surrounds(root))
        {
            return false;
        }
    }

    rec.t = root;
    rec.p = r.At(rec.t);
    glm::vec3 outwardNormal = (rec.p - currentCenter) / m_radius;
    rec.setFaceNormal(r, outwardNormal);
    rec.mat = m_mat;

    return true;
}