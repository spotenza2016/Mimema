#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>
// todo get rid of these std
using namespace std;

class Level {
public:
    class LevelState {
        float ambientLightIntensity = 0.1f;
        float lightIntensity = 0.8f;
        glm::vec3 lightVec = glm::normalize(glm::vec3(0, -1, -1.0f));

    public:
        vector<glm::vec3> lightPositions = {{0, 0, 0}, {80, 0, 80}};
        vector<float> lightIntensities = {0.3f, 0.2f};

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