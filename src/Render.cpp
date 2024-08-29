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

    MaterialPtr materialGround = std::make_shared<Lambertian>(glm::vec3(0.8f, 0.8f, 0.f));
    MaterialPtr materialCenter = std::make_shared<Lambertian>(glm::vec3(0.1f, 0.2f, 0.5f));
    MaterialPtr materialLeft = std::make_shared<Dielectric>(1.5f);
    MaterialPtr materialBubble = std::make_shared<Dielectric>(1.f / 1.5f);
    MaterialPtr materialRight = std::make_shared<Metal>(glm::vec3(0.8f, 0.6f, 0.2f), 1.f);

    world.Add(std::make_shared<Sphere>(glm::vec3(0.f, -100.5f, -1.f), 100.f, materialGround));
    world.Add(std::make_shared<Sphere>(glm::vec3(0.f, 0.f, -1.2f), 0.5f, materialCenter));
    world.Add(std::make_shared<Sphere>(glm::vec3(-1.f, 0.f, -1.f), 0.5f, materialLeft));
    world.Add(std::make_shared<Sphere>(glm::vec3(-1.f, 0.f, -1.f), 0.4f, materialBubble));
    world.Add(std::make_shared<Sphere>(glm::vec3(1.f, 0.f, -1.f), 0.5f, materialRight));

    Camera cam;
    cam.aspectRatio = ASPECT_RATIO;
    cam.imageWidth = IMAGE_WIDTH;
    cam.samplesPerPixel = SAMPLES_PER_PIXEL;
    cam.maxDepth = MAX_DEPTH;

    cam.vFov = 90;
    cam.lookFrom = glm::vec3(-2.f, 2.f, 1.f);
    cam.lookAt = glm::vec3(0.f, 0.f, -1.f);
    cam.vUp = glm::vec3(0.f, 1.f, 0.f);

    auto startTime = std::chrono::high_resolution_clock::now();
    auto image = cam.Render(world);
    auto duration = std::chrono::high_resolution_clock::now() - startTime;
    INFO("Total time: {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());

    return image;
}