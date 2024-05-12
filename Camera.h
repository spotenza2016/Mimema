#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

// Simple Camera Class
class Camera {
public:
    class CameraState {
        // View Matrix
        glm::vec3 cameraPosition = glm::vec3(0, 0, -1);
        glm::vec3 cameraTarget = glm::vec3(0, 0, 0);
        glm::vec3 upVec = glm::vec3(0, 1, 0);

        // Projection Matrix
        float fov = 70;
        float nearClippingPlane = 0.1f;
        float farClippingPlane = 100.0f;
        float aspectRatio = 1.0;

    public:

        // Getters
        glm::mat4 getViewProjectionMatrix() const;
        glm::mat4 static calculateViewProjectionMatrix(const glm::vec3& cameraPosition, const glm::vec3& cameraTarget,
                                                       const glm::vec3& upVec, float fov, float nearClippingPlane,
                                                       float farClippingPlane, float aspectRatio);
        const glm::vec3& getCameraPosition() const;
        const glm::vec3& getCameraTarget() const;
        const glm::vec3& getUpVec() const;
        float getFOV();
        float getNearClippingPlane();
        float getFarClippingPlane();
        float getAspectRatio();

        // Setters
        void setCameraPosition(const glm::vec3& cameraPosition);
        void setCameraTarget(const glm::vec3& cameraTarget);
        void setUpVec(const glm::vec3& upVec);
        void setFOV(float fov);
        void setNearClippingPlane(float nearClippingPlane);
        void setFarClippingPlane(float farClippingPlane);
        void setAspectRatio(float aspectRatio);
    };

private:
    CameraState cameraState;

public:
    CameraState& getCameraState();
};