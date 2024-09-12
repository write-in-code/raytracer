#pragma once

class Ray
{
public:
    Ray() = default;
    Ray(const glm::vec3 &origin, const glm::vec3 &direction)
        : m_origin(origin),
          m_direction(direction),
          m_time(0.f)
    {
    }

    Ray(const glm::vec3 &origin, const glm::vec3 &direction, float time)
        : m_origin(origin),
          m_direction(direction),
          m_time(time)
    {
    }

    const glm::vec3 &Origin() const { return m_origin; }
    const glm::vec3 &Direction() const { return m_direction; }
    float Time() const { return m_time; }
    glm::vec3 At(float t) const { return m_origin + t * m_direction; }

private:
    glm::vec3 m_origin;
    glm::vec3 m_direction;
    float m_time;
};