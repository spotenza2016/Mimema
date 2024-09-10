#include "RenderState.h"
Mimema::RenderState Mimema::RenderState::lerp(const Mimema::RenderState& prev,
                                              const Mimema::RenderState& curr, float alpha) {
    glm::vec4 backgroundColor = prev.backgroundColor * (1 - alpha) + curr.backgroundColor * (alpha);
    glm::vec3 ambientLightColor = prev.ambientLightColor * (1 - alpha) + curr.ambientLightColor * (alpha);
    float ambientLightIntensity = prev.ambientLightIntensity * (1 - alpha) + curr.ambientLightIntensity * (alpha);

    Camera::CameraState cameraState = prev.cameraState * (1 - alpha) + curr.cameraState * (alpha);

    std::unordered_map<Object*, Object::ObjectState> objectStates;
    for (auto iter = curr.objectStates.begin(); iter != curr.objectStates.end(); iter++) {
        if (prev.objectStates.count(iter->first) > 0) {
            objectStates[iter->first] = prev.objectStates.at(iter->first) * (1 - alpha) + iter->second * (alpha);
        }
    }

    std::unordered_map<DirectionalLight*, DirectionalLight::DirectionalLightState> directionalLightStates;
    for (auto iter = curr.directionalLightStates.begin(); iter != curr.directionalLightStates.end(); iter++) {
        if (prev.directionalLightStates.count(iter->first) > 0) {
            directionalLightStates[iter->first] = prev.directionalLightStates.at(iter->first) * (1 - alpha) + iter->second * (alpha);
        }
    }

    std::unordered_map<PointLight*, PointLight::PointLightState> pointLightStates;
    for (auto iter = curr.pointLightStates.begin(); iter != curr.pointLightStates.end(); iter++) {
        if (prev.pointLightStates.count(iter->first) > 0) {
            pointLightStates[iter->first] = prev.pointLightStates.at(iter->first) * (1 - alpha) + iter->second * (alpha);
        }
    }

    RenderState interpolatedState;
    interpolatedState.backgroundColor = backgroundColor;
    interpolatedState.ambientLightColor = ambientLightColor;
    interpolatedState.ambientLightIntensity = ambientLightIntensity;
    interpolatedState.cameraState = cameraState;
    interpolatedState.objectStates = objectStates;
    interpolatedState.directionalLightStates = directionalLightStates;
    interpolatedState.pointLightStates = pointLightStates;

    return interpolatedState;
}
