#include "Pch.h"
#include "Render.h"
#include "Camera.h"
#include "Quad.h"
#include "Sphere.h"
#include "BVHNode.h"
#include "Texture.h"

#define BOUNCING_SPHERES  1
#define CHECKERED_SPHERES 2
#define EARTH             3
#define PERLIN_SPHERES    4
#define QUADS             5
#define SCENE             QUADS

ImageInfo Render()
{
#if SCENE == BOUNCING_SPHERES
    HittableList world;

    TexturePtr checker = std::make_shared<CheckerTexture>(0.32f, glm::vec3(.2f, .3f, .1f), glm::vec3(.9f));
    world.Add(std::make_shared<Sphere>(glm::vec3(0.f, -1000.f, 0.f), 1000.f, std::make_shared<Lambertian>(checker)));

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

    world = HittableList(std::make_shared<BVHNode>(world));

    Camera cam;
    cam.aspectRatio = 16.f / 9.f;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.vFov = 20;
    cam.lookFrom = glm::vec3(13.f, 2.f, 3.f);
    cam.lookAt = glm::vec3(0.f);
    cam.vUp = glm::vec3(0.f, 1.f, 0.f);

    cam.defocusAngle = 0.6f;
    cam.focusDist = 10.f;

    auto image = cam.Render(world);
#elif SCENE == CHECKERED_SPHERES
    HittableList world;

    TexturePtr checker = std::make_shared<CheckerTexture>(0.32f, glm::vec3(.2f, .3f, .1f), glm::vec3(.9f));
    world.Add(std::make_shared<Sphere>(glm::vec3(0.f, -10.f, 0.f), 10.f, std::make_shared<Lambertian>(checker)));
    world.Add(std::make_shared<Sphere>(glm::vec3(0.f, 10.f, 0.f), 10.f, std::make_shared<Lambertian>(checker)));

    Camera cam;
    cam.aspectRatio = 16.f / 9.f;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.vFov = 20;
    cam.lookFrom = glm::vec3(13.f, 2.f, 3.f);
    cam.lookAt = glm::vec3(0.f);
    cam.vUp = glm::vec3(0.f, 1.f, 0.f);

    cam.defocusAngle = 0.f;

    auto image = cam.Render(world);
#elif SCENE == EARTH
    TexturePtr earthTexture = std::make_shared<ImageTexture>("assets/earthmap.jpg");
    MaterialPtr earthSurface = std::make_shared<Lambertian>(earthTexture);
    HittablePtr globe = std::make_shared<Sphere>(glm::vec3(0.f), 2.f, earthSurface);

    Camera cam;
    cam.aspectRatio = 16.f / 9.f;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.vFov = 20;
    cam.lookFrom = glm::vec3(0.f, 0.f, 12.f);
    cam.lookAt = glm::vec3(0.f);
    cam.vUp = glm::vec3(0.f, 1.f, 0.f);

    cam.defocusAngle = 0.f;

    auto image = cam.Render(HittableList(globe));
#elif SCENE == PERLIN_SPHERES
    HittableList world;

    TexturePtr pertext = std::make_shared<NoiseTexture>(4.f);
    world.Add(std::make_shared<Sphere>(glm::vec3(0.f, -1000.f, 0.f), 1000.f, std::make_shared<Lambertian>(pertext)));
    world.Add(std::make_shared<Sphere>(glm::vec3(0.f, 2.f, 0.f), 2.f, std::make_shared<Lambertian>(pertext)));

    Camera cam;
    cam.aspectRatio = 16.f / 9.f;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.vFov = 20;
    cam.lookFrom = glm::vec3(13.f, 2.f, 3.f);
    cam.lookAt = glm::vec3(0.f);
    cam.vUp = glm::vec3(0.f, 1.f, 0.f);

    cam.defocusAngle = 0.f;

    auto image = cam.Render(world);
#elif SCENE == QUADS
    HittableList world;

    MaterialPtr leftRed = std::make_shared<Lambertian>(glm::vec3(1.f, 0.2f, 0.2f));
    MaterialPtr blackGreen = std::make_shared<Lambertian>(glm::vec3(0.2f, 1.f, 0.2f));
    MaterialPtr rightBlue = std::make_shared<Lambertian>(glm::vec3(0.2f, 0.2f, 1.f));
    MaterialPtr upperOrange = std::make_shared<Lambertian>(glm::vec3(1.f, 0.5f, 0.f));
    MaterialPtr lowerTeal = std::make_shared<Lambertian>(glm::vec3(0.2f, 0.8f, 0.8f));

    world.Add(std::make_shared<Quad>(glm::vec3(-3.f, -2.f, 5.f), glm::vec3(0.f, 0.f, -4.f), glm::vec3(0.f, 4.f, 0.f), leftRed));
    world.Add(std::make_shared<Quad>(glm::vec3(-2.f, -2.f, 0.f), glm::vec3(4.f, 0.f, 0.f), glm::vec3(0.f, 4.f, 0.f), blackGreen));
    world.Add(std::make_shared<Quad>(glm::vec3(3.f, -2.f, 1.f), glm::vec3(0.f, 0.f, 4.f), glm::vec3(0.f, 4.f, 0.f), rightBlue));
    world.Add(std::make_shared<Quad>(glm::vec3(-2.f, 3.f, 1.f), glm::vec3(4.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 4.f), upperOrange));
    world.Add(std::make_shared<Quad>(glm::vec3(-2.f, -3.f, 5.f), glm::vec3(4.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -4.f), lowerTeal));

    Camera cam;
    cam.aspectRatio = 1.f;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.vFov = 80;
    cam.lookFrom = glm::vec3(0.f, 0.f, 9.f);
    cam.lookAt = glm::vec3(0.f);
    cam.vUp = glm::vec3(0.f, 1.f, 0.f);

    cam.defocusAngle = 0.f;

    auto image = cam.Render(world);
#else
    #error "Invalid scene"
#endif

    return image;
}