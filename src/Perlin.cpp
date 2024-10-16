#include "Pch.h"
#include "Perlin.h"

static float PerlinInterp(const glm::vec3 c[2][2][2], float u, float v, float w)
{
    float uu = u * u * (3.f - 2.f * u);
    float vv = v * v * (3.f - 2.f * v);
    float ww = w * w * (3.f - 2.f * w);

    float accum = 0.f;
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            for (int k = 0; k < 2; ++k)
            {
                glm::vec3 weightV(u - i, v - j, w - k);
                accum += (i * uu + (1 - i) * (1 - uu)) *
                         (j * vv + (1 - j) * (1 - vv)) *
                         (k * ww + (1 - k) * (1 - ww)) *
                         glm::dot(c[i][j][k], weightV);
            }
        }
    }

    return accum;
}

Perlin::Perlin()
{
    for (int i = 0; i < kPointCount; ++i)
    {
        m_randVec[i] = glm::normalize(glm::vec3(RandomFloat(-1.f, 1.f), RandomFloat(-1.f, 1.f), RandomFloat(-1.f, 1.f)));
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

    auto i = int(glm::floor(p.x)) & 255;
    auto j = int(glm::floor(p.y)) & 255;
    auto k = int(glm::floor(p.z)) & 255;
    glm::vec3 c[2][2][2];

    for (int di = 0; di < 2; ++di)
    {
        for (int dj = 0; dj < 2; ++dj)
        {
            for (int dk = 0; dk < 2; ++dk)
            {
                c[di][dj][dk] = m_randVec[m_permX[(i + di) & 255] ^
                                          m_permY[(j + dj) & 255] ^
                                          m_permZ[(k + dk) & 255]];
            }
        }
    }

    return PerlinInterp(c, u, v, w);
}

float Perlin::Turb(const glm::vec3 &p, int depth) const
{
    float accum = 0.f;
    glm::vec3 tempP = p;
    float weight = 1.f;

    for (int i = 0; i < depth; ++i)
    {
        accum += weight * Noise(tempP);
        weight *= 0.5f;
        tempP *= 2.f;
    }

    return glm::abs(accum);
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