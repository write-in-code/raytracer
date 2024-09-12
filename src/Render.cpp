#include "Pch.h"
#include "Render.h"
#include "Camera.h"
#include "Sphere.h"

constexpr float ASPECT_RATIO = 16.f / 9.f;
constexpr int IMAGE_WIDTH = 400;
constexpr int SAMPLES_PER_PIXEL = 100;
constexpr int MAX_DEPTH = 50;

ImageInfo Render()
{
    HittableList world;

    MaterialPtr groundMaterial = std::make_shared<Lambertian>(glm::vec3(0.5f));
    world.Add(std::make_shared<Sphere>(glm::vec3(0.f, -1000.f, 0.f), 1000.f, groundMaterial));

    for (int a = -11; a < 11; ++a)
    {
        for (int b = -11; b < 11; ++b)
        {
            float chooseMat = RandomFloat();
            glm::vec3 center(a + 0.9f * RandomFloat(), 0.2f, b + 0.9f * RandomFloat());

            if ((center - glm::vec3(4.f, 0.2f, 0.f)).length() > 0.9f)
            {
                MaterialPtr sphereMaterial;

                if (chooseMat < 0.8)
                {
                    glm::vec3 albedo = RandomVec() * RandomVec();
                    sphereMaterial = std::make_shared<Lambertian>(albedo);
                    glm::vec3 center2 = center + glm::vec3(0.f, RandomFloat(0.f, 0.5f), 0.f);
                    world.Add(std::make_shared<Sphere>(center, center2, 0.2f, sphereMaterial));
                }
                else if (chooseMat < 0.95)
                {
                    glm::vec3 albedo = RandomVec(0.5f, 1.f);
                    float fuzz = RandomFloat(0.f, 0.5f);
                    sphereMaterial = std::make_shared<Metal>(albedo, fuzz);
                    world.Add(std::make_shared<Sphere>(center, 0.2f, sphereMaterial));
                }
                else
                {
                    sphereMaterial = std::make_shared<Dielectric>(1.5f);
                    world.Add(std::make_shared<Sphere>(center, 0.2f, sphereMaterial));
                }
            }
        }
    }

    MaterialPtr material1 = std::make_shared<Dielectric>(1.5f);
    world.Add(std::make_shared<Sphere>(glm::vec3(0.f, 1.f, 0.f), 1.f, material1));

    MaterialPtr material2 = std::make_shared<Lambertian>(glm::vec3(0.4f, 0.2f, 0.1f));
    world.Add(std::make_shared<Sphere>(glm::vec3(-4.f, 1.f, 0.f), 1.f, material2));

    MaterialPtr material3 = std::make_shared<Metal>(glm::vec3(0.7f, 0.6f, 0.5f), 0.f);
    world.Add(std::make_shared<Sphere>(glm::vec3(4.f, 1.f, 0.f), 1.f, material3));

    Camera cam;
    cam.aspectRatio = ASPECT_RATIO;
    cam.imageWidth = IMAGE_WIDTH;
    cam.samplesPerPixel = SAMPLES_PER_PIXEL;
    cam.maxDepth = MAX_DEPTH;

    cam.vFov = 20;
    cam.lookFrom = glm::vec3(13.f, 2.f, 3.f);
    cam.lookAt = glm::vec3(0.f);
    cam.vUp = glm::vec3(0.f, 1.f, 0.f);

    cam.defocusAngle = 0.6f;
    cam.focusDist = 10.f;

    auto startTime = std::chrono::high_resolution_clock::now();
    auto image = cam.Render(world);
    auto duration = std::chrono::high_resolution_clock::now() - startTime;
    INFO("Total time: {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());

    return image;
}