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
    float vFov = 90;
    glm::vec3 lookFrom = glm::vec3(0.f);
    glm::vec3 lookAt = glm::vec3(0.f, 0.f, -1.f);
    glm::vec3 vUp = glm::vec3(0.f, 1.f, 0.f);
    float defocusAngle = 0.f;
    float focusDist = 10.f;

private:
    void Initialize();
    Ray GetRay(int i, int j) const;
    glm::vec3 SampleSquare() const;
    glm::vec3 RayColor(const Ray &r, int depth, const Hittable &world) const;

    int m_imageHeight;
    float m_pixelSamplesScale;
    glm::vec3 m_center;
    glm::vec3 m_pixel00Loc;
    glm::vec3 m_pixelDeltaU;
    glm::vec3 m_pixelDeltaV;
    glm::vec3 m_u, m_v, m_w;
    glm::vec3 m_defocusDiskU;
    glm::vec3 m_defocusDiskV;
};