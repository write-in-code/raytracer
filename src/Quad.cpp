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
    m_w = n / glm::dot(n, n);

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
    glm::vec3 planarHitptVector = intersection - m_Q;
    float alpha = glm::dot(m_w, glm::cross(planarHitptVector, m_v));
    float beta = glm::dot(m_w, glm::cross(m_u, planarHitptVector));

    if (!IsInterior(alpha, beta, rec))
    {
        return false;
    }

    rec.t = t;
    rec.p = intersection;
    rec.mat = m_mat;
    rec.SetFaceNormal(r, m_normal);

    return true;
}

bool Quad::IsInterior(float a, float b, HitRecord &rec) const
{
    Interval unitInterval = Interval(0.f, 1.f);

    if (!unitInterval.Contains(a) || !unitInterval.Contains(b))
    {
        return false;
    }

    rec.u = a;
    rec.v = b;
    return true;
}