#pragma once

class Perlin
{
public:
    Perlin();
    float Noise(const glm::vec3 &p) const;

private:
    static void PerlinGeneratePerm(int *p);
    static void Permute(int *p, int n);

    static constexpr int kPointCount = 256;
    glm::vec3 m_randVec[kPointCount];
    int m_permX[kPointCount];
    int m_permY[kPointCount];
    int m_permZ[kPointCount];
};