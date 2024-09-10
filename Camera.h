#ifndef MIMEMA_CAMERA_H
#define MIMEMA_CAMERA_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "tracy/Tracy.hpp"

namespace Mimema {
    struct Camera {
        struct CameraState {
            // View Matrix
            glm::vec3 cameraPosition = {0, 10, 0};
            glm::vec3 cameraTarget = {0, 10, -1};
            glm::vec3 upVec = {0, 1, 0};

            // Projection Matrix
            float fov = 70;
            float nearClippingPlane = 1.0f;
            float farClippingPlane = 1000.0f;
            float aspectRatio = 1;

            CameraState() = default;

            CameraState(const glm::vec3& cameraPosition, const glm::vec3& cameraTarget, const glm::vec3& upVec,
                   float fov, float nearClippingPlane, float farClippingPlane, float aspectRatio);

            CameraState operator*(float alpha) const;

            CameraState operator+(const CameraState& rhs) const;
        };

        CameraState cameraState;

        explicit Camera(CameraState cameraState);
    };
}

#endif