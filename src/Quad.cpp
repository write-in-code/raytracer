#include "Pch.h"
#include "Quad.h"

Quad::Quad(const glm::vec3 &Q, const glm::vec3 &u, const glm::vec3 &v, const MaterialPtr &mat)
    : m_Q(Q),
      m_u(u),
      m_v(v),
      m_mat(mat)
{
    glm::vec3 n = glm::cross(u, v);
    m_normal = glm::normalize(n);
    m_D = glm::dot(m_normal, m_Q);

    SetBoundingBox();
}

void Quad::SetBoundingBox()
{
    AABB bboxDiagonal1 = AABB(m_Q, m_Q + m_u + m_v);
    AABB bboxDiagonal2 = AABB(m_Q + m_u, m_Q + m_v);
    m_bbox = AABB(bboxDiagonal1, bboxDiagonal2);
}

bool Quad::Hit(const Ray &r, Interval rayT, HitRecord &rec) const
{
    float denom = glm::dot(m_normal, r.Direction());

    if (glm::abs(denom) < 1e-6f)
    {
        return false;
    }

    float t = (m_D - glm::dot(m_normal, r.Origin())) / denom;
    if (!rayT.Contains(t))
    {
        return false;
    }

    glm::vec3 intersection = r.At(t);

    rec.t = t;
    rec.p = intersection;
    rec.mat = m_mat;
    rec.SetFaceNormal(r, m_normal);

    return true;
}