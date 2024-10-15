#include "Pch.h"
#include "Perlin.h"

static float TrilinearInterp(float c[2][2][2], float u, float v, float w)
{
    float accum = 0.f;
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            for (int k = 0; k < 2; ++k)
            {
                accum += (i * u + (1 - i) * (1 - u)) *
                         (j * v + (1 - j) * (1 - v)) *
                         (k * w + (1 - k) * (1 - w)) *
                         c[i][j][k];
            }
        }
    }

    return accum;
}

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
    auto u = p.x - glm::floor(p.x);
    auto v = p.y - glm::floor(p.y);
    auto w = p.z - glm::floor(p.z);
    u = u * u * (3 - 2 * u);
    v = v * v * (3 - 2 * v);
    w = w * w * (3 - 2 * w);

    auto i = int(glm::floor(p.x)) & 255;
    auto j = int(glm::floor(p.y)) & 255;
    auto k = int(glm::floor(p.z)) & 255;
    float c[2][2][2];

    for (int di = 0; di < 2; ++di)
    {
        for (int dj = 0; dj < 2; ++dj)
        {
            for (int dk = 0; dk < 2; ++dk)
            {
                c[di][dj][dk] = m_randFloat[m_permX[(i + di) & 255] ^
                                            m_permY[(j + dj) & 255] ^
                                            m_permZ[(k + dk) & 255]];
            }
        }
    }

    return TrilinearInterp(c, u, v, w);
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