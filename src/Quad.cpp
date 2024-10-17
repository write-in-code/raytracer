#include "Pch.h"
#include "Quad.h"

Quad::Quad(const glm::vec3 &q, const glm::vec3 &u, const glm::vec3 &v, const MaterialPtr &mat)
    : m_q(q),
      m_u(u),
      m_v(v),
      m_mat(mat)
{
    SetBoundingBox();
}

void Quad::SetBoundingBox()
{
    AABB bboxDiagonal1 = AABB(m_q, m_q + m_u + m_v);
    AABB bboxDiagonal2 = AABB(m_q + m_u, m_q + m_v);
    m_bbox = AABB(bboxDiagonal1, bboxDiagonal2);
}