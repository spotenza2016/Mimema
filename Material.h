#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/mat4x4.hpp>
#include <string>
#include <iostream>
using namespace std;

// This class needs to be reworked to correspond better with mtl files and have proper initialization, for now is a bit jank
class Material {
    unsigned int texture;
    glm::vec3 kd = glm::vec3(1, 0, 1);
    string map_kd;
public:
    const glm::vec3& getKd();
    const string& getMap_kd();
    unsigned int getTexture();
    void setKd(const glm::vec3& kd);
    void setMap_kd(const string& map_kd);
    Material();
    ~Material();
};