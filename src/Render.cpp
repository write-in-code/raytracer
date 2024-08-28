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

    float R = glm::cos(glm::pi<float>() / 4.f);
    MaterialPtr materialLeft = std::make_shared<Lambertian>(glm::vec3(0.f, 0.f, 1.f));
    MaterialPtr materialRight = std::make_shared<Lambertian>(glm::vec3(1.f, 0.f, 0.f));
    
    world.Add(std::make_shared<Sphere>(glm::vec3(-R, 0.f, -1.f), R, materialLeft));
    world.Add(std::make_shared<Sphere>(glm::vec3(R, 0.f, -1.f), R, materialRight));

    Camera cam;
    cam.aspectRatio = ASPECT_RATIO;
    cam.imageWidth = IMAGE_WIDTH;
    cam.samplesPerPixel = SAMPLES_PER_PIXEL;
    cam.maxDepth = MAX_DEPTH;

    auto startTime = std::chrono::high_resolution_clock::now();
    auto image = cam.Render(world);
    auto duration = std::chrono::high_resolution_clock::now() - startTime;
    INFO("Total time: {} ms", std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());

    return image;
}