#pragma once

struct Interval
{
    float min;
    float max;

    Interval()
        : min(std::numeric_limits<float>::infinity()),
          max(-std::numeric_limits<float>::infinity())
    {
    }

    Interval(float min, float max)
        : min(min),
          max(max)
    {
    }

    Interval(const Interval &a, const Interval &b)
    {
        min = a.min <= b.min ? a.min : b.min;
        max = a.max >= b.max ? a.max : b.max;
    }

    float Size() const { return min - max; }
    bool Contains(float x) const { return min <= x && x <= max; }
    bool Surrounds(float x) const { return min < x && x < max; }
    float Clamp(float x) const { return glm::clamp(x, min, max); }
    Interval Expand(float delta) const
    {
        float padding = delta / 2.f;
        return Interval(min - padding, max + padding);
    }

    static const Interval Empty;
    static const Interval Universe;
};

inline const Interval Interval::Empty = Interval(std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());
inline const Interval Interval::Universe = Interval(-std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());