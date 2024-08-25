#include "Pch.h"
#include "Camera.h"

ImageInfo Camera::Render(const Hittable &world)
{
    Initialize();

    auto data = std::make_unique<glm::u8vec4[]>(imageWidth * imageHeight);
    for (int j = 0; j < imageHeight; ++j)
    {
        for (int i = 0; i < imageWidth; ++i)
        {
            if (!i && j % 10 == 0)
            {
                INFO("{:.1f}", static_cast<float>(j * imageWidth + i) / (imageWidth * imageHeight) * 100.f);
            }

            glm::vec3 pixelColor(0.f);
            for (int sample = 0; sample < samplesPerPixel; ++sample)
            {
                Ray r = GetRay(i, j);
                pixelColor += RayColor(r, maxDepth, world);
            }

            glm::vec3 sampledColor = glm::clamp(pixelSamplesScale * pixelColor, glm::vec3(0.f), glm::vec3(1.f));
            glm::vec3 gammaCorrectedSampledColor = glm::clamp(LinearToGamma(sampledColor), glm::vec3(0.f), glm::vec3(1.f));
            glm::vec3 scaledColor = gammaCorrectedSampledColor * 255.f;
            glm::u8vec4 finalColor = glm::uvec4(static_cast<glm::u8>(scaledColor.x), static_cast<glm::u8>(scaledColor.y), static_cast<glm::u8>(scaledColor.z), 255);
            data[j * imageWidth + i] = finalColor;
        }
    }

    return ImageInfo{.data = std::move(data), .width = imageWidth, .height = imageHeight};
}

void Camera::Initialize()
{
    imageHeight = int(imageWidth / aspectRatio);
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;

    pixelSamplesScale = 1.f / samplesPerPixel;
    center = glm::vec3(0.f);

    auto focalLength = 1.f;
    auto viewportHeight = 2.f;
    auto viewportWidth = viewportHeight * (float(imageWidth) / imageHeight);

    auto viewportU = glm::vec3(viewportWidth, 0, 0);
    auto viewportV = glm::vec3(0, -viewportHeight, 0);

    pixelDeltaU = viewportU / static_cast<float>(imageWidth);
    pixelDeltaV = viewportV / static_cast<float>(imageHeight);

    auto viewportUpperLeft = center - glm::vec3(0, 0, focalLength) - viewportU / 2.f - viewportV / 2.f;
    pixel00Loc = viewportUpperLeft + 0.5f * (pixelDeltaU + pixelDeltaV);
}

Ray Camera::GetRay(int i, int j) const
{
    glm::vec3 offset = SampleSquare();
    glm::vec3 pixelSample = pixel00Loc + ((i + offset.x) * pixelDeltaU) + ((j + offset.y) * pixelDeltaV);

    glm::vec3 rayOrigin = center;
    glm::vec3 rayDir = pixelSample - rayOrigin;

    return Ray(rayOrigin, rayDir);
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