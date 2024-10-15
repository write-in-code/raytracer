#include "Pch.h"
#include "Camera.h"

ImageInfo Camera::Render(const Hittable &world)
{
    Initialize();
    _profblock();

    auto data = std::make_unique<glm::u8vec4[]>(imageWidth * m_imageHeight);
    for (int j = 0; j < m_imageHeight; ++j)
    {
        for (int i = 0; i < imageWidth; ++i)
        {
            if (!i && j % 10 == 0)
            {
                INFO("{:.1f}", static_cast<float>(j * imageWidth + i) / (imageWidth * m_imageHeight) * 100.f);
            }

            glm::vec3 pixelColor(0.f);
            for (int sample = 0; sample < samplesPerPixel; ++sample)
            {
                Ray r = GetRay(i, j);
                pixelColor += RayColor(r, maxDepth, world);
            }

            glm::vec3 sampledColor = glm::clamp(m_pixelSamplesScale * pixelColor, glm::vec3(0.f), glm::vec3(1.f));
            glm::vec3 gammaCorrectedSampledColor = glm::clamp(LinearToGamma(sampledColor), glm::vec3(0.f), glm::vec3(1.f));
            glm::vec3 scaledColor = gammaCorrectedSampledColor * 255.f;
            glm::u8vec4 finalColor = glm::uvec4(static_cast<glm::u8>(scaledColor.x), static_cast<glm::u8>(scaledColor.y), static_cast<glm::u8>(scaledColor.z), 255);
            data[j * imageWidth + i] = finalColor;
        }
    }

    return ImageInfo{.data = std::move(data), .width = imageWidth, .height = m_imageHeight};
}

void Camera::Initialize()
{
    m_imageHeight = int(imageWidth / aspectRatio);
    m_imageHeight = (m_imageHeight < 1) ? 1 : m_imageHeight;

    m_pixelSamplesScale = 1.f / samplesPerPixel;
    m_center = lookFrom;

    float theta = glm::radians(vFov);
    float h = glm::tan(theta / 2.f);
    float viewportHeight = 2.f * h * focusDist;
    float viewportWidth = viewportHeight * (float(imageWidth) / m_imageHeight);

    m_w = glm::normalize(lookFrom - lookAt);
    m_u = glm::normalize(glm::cross(vUp, m_w));
    m_v = glm::cross(m_w, m_u);

    glm::vec3 viewportU = viewportWidth * m_u;
    glm::vec3 viewportV = viewportHeight * -m_v;

    m_pixelDeltaU = viewportU / static_cast<float>(imageWidth);
    m_pixelDeltaV = viewportV / static_cast<float>(m_imageHeight);

    glm::vec3 viewportUpperLeft = m_center - (focusDist * m_w) - viewportU / 2.f - viewportV / 2.f;
    m_pixel00Loc = viewportUpperLeft + 0.5f * (m_pixelDeltaU + m_pixelDeltaV);

    float defocusRadius = focusDist * glm::tan(glm::radians(defocusAngle / 2.f));
    m_defocusDiskU = m_u * defocusRadius;
    m_defocusDiskV = m_v * defocusRadius;
}

Ray Camera::GetRay(int i, int j) const
{
    glm::vec3 offset = SampleSquare();
    glm::vec3 pixelSample = m_pixel00Loc + ((i + offset.x) * m_pixelDeltaU) + ((j + offset.y) * m_pixelDeltaV);

    auto defocusDiskSample = [this]
    {
        glm::vec3 p = RandomInUnitDisk();
        return m_center + (p.x * m_defocusDiskU) + (p.y * m_defocusDiskV);
    };

    glm::vec3 rayOrigin = (defocusAngle <= 0.f) ? m_center : defocusDiskSample();
    glm::vec3 rayDir = pixelSample - rayOrigin;
    float rayTime = RandomFloat();

    return Ray(rayOrigin, rayDir, rayTime);
}

glm::vec3 Camera::SampleSquare() const
{
    return glm::vec3(RandomFloat() - 0.5f, RandomFloat() - 0.5f, 0.f);
}

glm::vec3 Camera::RayColor(const Ray &r, int depth, const Hittable &world) const
{
    if (depth == 0)
    {
        return glm::vec3(0.f);
    }

    HitRecord rec;
    if (world.Hit(r, Interval(0.001f, std::numeric_limits<float>::infinity()), rec))
    {
        Ray scattered;
        glm::vec3 attenuation;
        if (rec.mat->Scatter(r, rec, attenuation, scattered))
        {
            return attenuation * RayColor(scattered, depth - 1, world);
        }

        return glm::vec3(0.f);
    }

    glm::vec3 unitDirection = glm::normalize(r.Direction());
    float a = 0.5f * (unitDirection.y + 1.0f);
    return glm::mix(glm::vec3(1.f), glm::vec3(0.5f, 0.7f, 1.f), a);
}