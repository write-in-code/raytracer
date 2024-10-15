#include "Pch.h"
#include "Perlin.h"

Perlin::Perlin()
{
    for (int i = 0; i < kPointCount; ++i)
    {
        m_randFloat[i] = RandomFloat();
    }

    PerlinGeneratePerm(m_permX);
    PerlinGeneratePerm(m_permY);
    PerlinGeneratePerm(m_permZ);
}

float Perlin::Noise(const glm::vec3 &p) const
{
    auto i = int(4 * p.x) & 255;
    auto j = int(4 * p.y) & 255;
    auto k = int(4 * p.z) & 255;

    return m_randFloat[m_permX[i] ^ m_permY[j] ^ m_permZ[k]];
}

void Perlin::PerlinGeneratePerm(int *p)
{
    for (int i = 0; i < kPointCount; ++i)
    {
        p[i] = i;
    }

    Permute(p, kPointCount);
}

void Perlin::Permute(int *p, int n)
{
    for (int i = n - 1; i > 0; --i)
    {
        int target = RandomInt(0, i);
        std::swap(p[i], p[target]);
    }
}