#ifndef MIMEMA_POINTLIGHT_H
#define MIMEMA_POINTLIGHT_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "tracy/Tracy.hpp"

namespace Mimema {
    struct PointLight {
        struct PointLightState {
            glm::vec3 position = {0, 0, 0};
            glm::vec3 color = {1, 1, 1};
            float intensity = 1;
            glm::vec3 attenuation = {1, 0, 0};

            PointLightState() = default;

            PointLightState(const glm::vec3& position, const glm::vec3& color, float intensity, const glm::vec3& attenuation);

            PointLightState operator*(float alpha) const;

            PointLightState operator+(const PointLightState& rhs) const;
        };
        PointLightState pointLightState;

        PointLight() = default;

        explicit PointLight(const PointLightState& pointLightState);
    };
}

#endif //MIMEMA_POINTLIGHT_H