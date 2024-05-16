#include "Camera.h"
Camera::CameraState& Camera::getCameraState() {
    return cameraState;
}

glm::mat4 Camera::CameraState::getViewMatrix() const {
    return CameraState::calculateViewMatrix(cameraPosition, cameraTarget, upVec);
}

glm::mat4 Camera::CameraState::getProjectionMatrix() const {
    return CameraState::calculateProjectionMatrix(fov, nearClippingPlane, farClippingPlane, aspectRatio);
}

glm::mat4 Camera::CameraState::calculateViewMatrix(const glm::vec3 &cameraPosition,
                                                   const glm::vec3 &cameraTarget, const glm::vec3 &upVec) {
    return glm::lookAt(cameraPosition, cameraTarget, upVec);
}

glm::mat4 Camera::CameraState::calculateProjectionMatrix(float fov, float nearClippingPlane, float farClippingPlane,
                                                         float aspectRatio) {
    return glm::perspective(glm::radians(fov), aspectRatio, nearClippingPlane, farClippingPlane);
}

const glm::vec3& Camera::CameraState::getCameraPosition() const {
    return cameraPosition;
}

const glm::vec3& Camera::CameraState::getCameraTarget() const {
    return cameraTarget;
}

const glm::vec3& Camera::CameraState::getUpVec() const {
    return upVec;
}

float Camera::CameraState::getFOV() {
    return fov;
}

float Camera::CameraState::getNearClippingPlane() {
    return nearClippingPlane;
}

float Camera::CameraState::getFarClippingPlane() {
    return farClippingPlane;
}

float Camera::CameraState::getAspectRatio() {
    return aspectRatio;
}

void Camera::CameraState::setCameraPosition(const glm::vec3& cameraPosition) {
    this->cameraPosition = cameraPosition;
}

void Camera::CameraState::setCameraTarget(const glm::vec3& cameraTarget) {
    this->cameraTarget = cameraTarget;
}

void Camera::CameraState::setUpVec(const glm::vec3& upVec) {
    this->upVec = upVec;
}

void Camera::CameraState::setFOV(float fov) {
    this->fov = fov;
}

void Camera::CameraState::setNearClippingPlane(float nearClippingPlane) {
    this->nearClippingPlane = nearClippingPlane;
}

void Camera::CameraState::setFarClippingPlane(float farClippingPlane) {
    this->farClippingPlane = farClippingPlane;
}

void Camera::CameraState::setAspectRatio(float aspectRatio) {
    this->aspectRatio = aspectRatio;
}