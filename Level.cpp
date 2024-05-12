#include "Level.h"

float Level::LevelState::getAmbientLightIntensity() const {
    return ambientLightIntensity;
}

float Level::LevelState::getLightIntensity() const {
    return lightIntensity;
}

const glm::vec3 &Level::LevelState::getLightVec() const {
    return lightVec;
}

void Level::LevelState::setAmbientLightIntensity(float ambientLightIntensity) {
    this->ambientLightIntensity = ambientLightIntensity;
}

void Level::LevelState::setLightIntensity(float lightIntensity) {
    this->lightIntensity = lightIntensity;
}

void Level::LevelState::setLightVec(const glm::vec3& lightVec) {
    this->lightVec = lightVec;
}

Level::LevelState& Level::getLevelState() {
    return levelState;
}
