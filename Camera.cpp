#include "Camera.h"
Mimema::Camera::CameraState::CameraState(const glm::vec3& cameraPosition, const glm::vec3& cameraTarget,
                                         const glm::vec3& upVec, float fov, float nearClippingPlane,
                                         float farClippingPlane, float aspectRatio) {
    this->cameraPosition = cameraPosition;
    this->cameraTarget = cameraTarget;
    this->upVec = upVec;
    this->fov = fov;
    this->nearClippingPlane = nearClippingPlane;
    this->farClippingPlane = farClippingPlane;
    this->aspectRatio = aspectRatio;
}

Mimema::Camera::CameraState
Mimema::Camera::CameraState::operator*(float alpha) const {
    return {cameraPosition * alpha,
            cameraTarget * alpha,
            upVec * alpha,
            fov * alpha,
            nearClippingPlane * alpha,
            farClippingPlane * alpha,
            aspectRatio * alpha};
}

Mimema::Camera::CameraState Mimema::Camera::CameraState::operator+(const Mimema::Camera::CameraState& rhs) const {
    return {cameraPosition + rhs.cameraPosition,
            cameraTarget + rhs.cameraTarget,
            upVec + rhs.upVec,
            fov + rhs.fov,
            nearClippingPlane + rhs.nearClippingPlane,
            farClippingPlane + rhs.farClippingPlane,
            aspectRatio + rhs.aspectRatio};
}

Mimema::Camera::Camera(Mimema::Camera::CameraState cameraState) : cameraState(cameraState) {
    // None
}
