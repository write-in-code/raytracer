#pragma once

#define USE_FASTER_RAND 0

#if !USE_FASTER_RAND
namespace detail
{
    struct Rand
    {
        std::random_device randomDevice;
        std::mt19937 randomGen{randomDevice()};
        std::uniform_real_distribution<float> uniformFloatDistribution;
        std::uniform_int_distribution<int> uniformIntDistribution;
    };

    inline Rand g_rand;
} // namespace detail
#endif

inline float RandomFloat()
{
#if USE_FASTER_RAND
    static constexpr float factor = 1.f / RAND_MAX;
    return rand() * factor;
#else
    return detail::g_rand.uniformFloatDistribution(detail::g_rand.randomGen);
#endif
}

inline float RandomFloat(float min, float max)
{
#if USE_FASTER_RAND
    return min + RandomFloat() * (max - min);
#else
    return std::uniform_real_distribution<float>(min, max)(detail::g_rand.randomGen);
#endif
}

inline int RandomInt(int min, int max)
{
#if USE_FASTER_RAND
    return min + rand() % (max - min + 1);
#else
    return std::uniform_int_distribution<int>(min, max)(detail::g_rand.randomGen);
#endif
}

inline glm::vec3 RandomVec()
{
    return glm::vec3(RandomFloat(), RandomFloat(), RandomFloat());
}

inline glm::vec3 RandomVec(float min, float max)
{
    return glm::vec3(RandomFloat(min, max), RandomFloat(min, max), RandomFloat(min, max));
}

inline glm::vec3 RandomInUnitSphere()
{
    while (true)
    {
        glm::vec3 p = RandomVec(-1.f, 1.f);
        float lengthSq = glm::dot(p, p);
        if (lengthSq < 1.f && glm::abs(lengthSq) > 0.0001f)
        {
            return p;
        }
    }
}

inline glm::vec3 RandomInUnitDisk()
{
    while (true)
    {
        auto p = glm::vec3(RandomFloat(-1.f, 1.f), RandomFloat(-1.f, 1.f), 0.f);
        if (glm::dot(p, p) < 1.f)
        {
            return p;
        }
    }
}

inline glm::vec3 RandomUnitVec()
{
    return glm::normalize(RandomInUnitSphere());
}

inline glm::vec3 RandomOnHemisphere(const glm::vec3 &normal)
{
    glm::vec3 onUnitSphere = RandomUnitVec();
    return glm::dot(onUnitSphere, normal) > 0.f ? onUnitSphere : -onUnitSphere;
}

inline bool NearZero(const glm::vec3 &v, float epsilon = 0.0001f)
{
    return glm::all(glm::epsilonEqual(v, glm::vec3(0.f), epsilon));
}

inline float LinearToGamma(float linearComponent)
{
    if (linearComponent > 0)
    {
        return glm::sqrt(linearComponent);
    }

    return 0;
}

inline glm::vec3 LinearToGamma(const glm::vec3 &linearComponent)
{
    return glm::vec3(LinearToGamma(linearComponent.x), LinearToGamma(linearComponent.y), LinearToGamma(linearComponent.z));
}
