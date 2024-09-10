#ifndef MIMEMA_DIRECTIONALLIGHT_H
#define MIMEMA_DIRECTIONALLIGHT_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "tracy/Tracy.hpp"

namespace Mimema {
    struct DirectionalLight {
        struct DirectionalLightState {
            glm::vec3 direction = {0, -1, 0};
            glm::vec3 color = {1, 1, 1};
            float intensity = 1;

            DirectionalLightState() = default;

            DirectionalLightState(const glm::vec3& direction, const glm::vec3& color, float intensity);

            DirectionalLightState operator*(float alpha) const;

            DirectionalLightState operator+(const DirectionalLightState& rhs) const;
        };
        DirectionalLightState directionalLightState;

        DirectionalLight() = default;

        explicit DirectionalLight(const DirectionalLightState& directionalLightState);
    };
}

#endif //MIMEMA_DIRECTIONALLIGHT_H
