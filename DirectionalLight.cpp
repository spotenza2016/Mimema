#include "DirectionalLight.h"

Mimema::DirectionalLight::DirectionalLightState::DirectionalLightState(const glm::vec3& direction,
                                                                       const glm::vec3& color, float intensity) {
    this->direction = direction;
    this->color = color;
    this->intensity = intensity;
}

Mimema::DirectionalLight::DirectionalLightState
Mimema::DirectionalLight::DirectionalLightState::operator*(float alpha) const {
    return {direction * alpha,
            color * alpha,
            intensity * alpha};
}

Mimema::DirectionalLight::DirectionalLightState
Mimema::DirectionalLight::DirectionalLightState::operator+(const Mimema::DirectionalLight::DirectionalLightState& rhs) const {
    return {direction + rhs.direction,
            color + rhs.color,
            intensity + rhs.intensity};
}

Mimema::DirectionalLight::DirectionalLight(const DirectionalLightState& directionalLightState) : directionalLightState(directionalLightState) {
    // None
}