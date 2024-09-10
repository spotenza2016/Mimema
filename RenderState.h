#ifndef MIMEMA_RENDERSTATE_H
#define MIMEMA_RENDERSTATE_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/mat4x4.hpp>
#include <unordered_map>

#include "tracy/Tracy.hpp"

#include "Camera.h"
#include "Object.h"
#include "PointLight.h"
#include "DirectionalLight.h"

namespace Mimema {
    struct RenderState {
        glm::vec4 backgroundColor = glm::vec4(0, 0, 0, 1);
        glm::vec3 ambientLightColor = {0, 0, 0};
        float ambientLightIntensity = 0.0f;
        Camera::CameraState cameraState;
        std::unordered_map<Object*, Object::ObjectState> objectStates;
        std::unordered_map<DirectionalLight*, DirectionalLight::DirectionalLightState> directionalLightStates;
        std::unordered_map<PointLight*, PointLight::PointLightState> pointLightStates;

        static RenderState lerp(const RenderState& prev, const RenderState& curr, float alpha);
    };
}

#endif // MIMEMA_RENDERSTATE_H