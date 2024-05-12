#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

class Level {
public:
    class LevelState {
        float ambientLightIntensity = 0.8f;
        float lightIntensity = 0.8f;
        glm::vec3 lightVec = glm::normalize(glm::vec3(0, 0, -1.0f));

    public:
        // Getters
        float getAmbientLightIntensity() const;
        float getLightIntensity()  const;
        const glm::vec3& getLightVec()  const;

        // Setters
        void setAmbientLightIntensity(float ambientLightIntensity);
        void setLightIntensity(float lightIntensity);
        void setLightVec(const glm::vec3& lightVec);
    };

private:
    LevelState levelState;

public:
    LevelState& getLevelState();
};