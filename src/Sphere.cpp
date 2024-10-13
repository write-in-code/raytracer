#include "Pch.h"
#include "Sphere.h"

Sphere::Sphere(const glm::vec3 &staticCenter, float radius, const MaterialPtr &mat)
    : m_center(staticCenter, glm::vec3(0.f)),
      m_radius(glm::max(0.f, radius)),
      m_mat(mat)
{
    glm::vec3 rvec = glm::vec3(radius);
    m_bbox = AABB(staticCenter - rvec, staticCenter + rvec);
}

Sphere::Sphere(const glm::vec3 &center1, const glm::vec3 &center2, float radius, const MaterialPtr &mat)
    : m_center(center1, (center2 - center1)),
      m_radius(glm::max(0.f, radius)),
      m_mat(mat)
{
    glm::vec3 rvec = glm::vec3(radius);
    AABB box1(m_center.At(0) - rvec, m_center.At(0) + rvec);
    AABB box2(m_center.At(1) - rvec, m_center.At(1) + rvec);
    m_bbox = AABB(box1, box2);
}

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
    GetSphereUV(outwardNormal, rec.u, rec.v);
    rec.mat = m_mat;

    return true;
}

void Sphere::GetSphereUV(const glm::vec3 &p, float &u, float &v)
{
    float theta = glm::acos(-p.y);
    float phi = glm::atan(-p.z, p.x) + glm::pi<float>();

    u = phi / (2.f * glm::pi<float>());
    v = theta / glm::pi<float>();
}