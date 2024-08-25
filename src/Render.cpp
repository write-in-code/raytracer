#include "Pch.h"
#include "Render.h"
#include "Camera.h"
#include "Sphere.h"

ImageInfo Render()
{
    HittableList world;
    MaterialPtr materialGround = std::make_shared<Lambertian>(glm::vec3(0.8f, 0.8f, 0.0f));
    MaterialPtr materialCenter = std::make_shared<Lambertian>(glm::vec3(0.1f, 0.2f, 0.5f));
    MaterialPtr materialLeft = std::make_shared<Metal>(glm::vec3(0.8f), 0.3f);
    MaterialPtr materialRight = std::make_shared<Metal>(glm::vec3(0.8f, 0.6f, 0.2f), 1.f);

    world.Add(std::make_shared<Sphere>(glm::vec3(0.0, -100.5, -1.0), 100.0, materialGround));
    world.Add(std::make_shared<Sphere>(glm::vec3(0.0, 0.0, -1.2), 0.5, materialCenter));
    world.Add(std::make_shared<Sphere>(glm::vec3(-1.0, 0.0, -1.0), 0.5, materialLeft));
    world.Add(std::make_shared<Sphere>(glm::vec3(1.0, 0.0, -1.0), 0.5, materialRight));

    Camera cam;
    cam.aspectRatio = 16.f / 9.f;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    auto startTime = std::chrono::high_resolution_clock::now();
    auto image = cam.Render(world);
    auto duration = std::chrono::high_resolution_clock::now() - startTime;
    INFO("Total time: {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());

    return image;
}