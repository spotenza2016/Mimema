#include "PointLight.h"

Mimema::PointLight::PointLightState::PointLightState(const glm::vec3& position, const glm::vec3& color,
                                                     float intensity, const glm::vec3& attenuation) {
    this->position = position;
    this->color = color;
    this->intensity = intensity;
    this->attenuation = attenuation;
}

Mimema::PointLight::PointLightState Mimema::PointLight::PointLightState::operator*(float alpha) const {
    return {position * alpha,
            color * alpha,
            intensity * alpha,
            attenuation * attenuation};
}

Mimema::PointLight::PointLightState
Mimema::PointLight::PointLightState::operator+(const Mimema::PointLight::PointLightState& rhs) const {
    return {position + rhs.position,
            color + rhs.color,
            intensity + rhs.intensity,
            attenuation + rhs.attenuation};
}

Mimema::PointLight::PointLight(const PointLightState& pointLightState) : pointLightState(pointLightState) {
    // None
}