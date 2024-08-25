#pragma once
#include "ImageDefs.h"
#include "HittableList.h"
#include "Material.h"

class Camera
{
public:
    ImageInfo Render(const Hittable &world);

    float aspectRatio = 1.f;
    int imageWidth = 100;
    int samplesPerPixel = 10;
    int maxDepth = 10;

private:
    void Initialize();
    Ray GetRay(int i, int j) const;
    glm::vec3 SampleSquare() const;
    glm::vec3 RayColor(const Ray &r, int depth, const Hittable &world) const;

    int imageHeight;
    float pixelSamplesScale;
    glm::vec3 center;
    glm::vec3 pixel00Loc;
    glm::vec3 pixelDeltaU;
    glm::vec3 pixelDeltaV;
};