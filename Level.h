#ifndef MIMEMA_LEVEL_H
#define MIMEMA_LEVEL_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include <list>

#include "tracy/Tracy.hpp"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "Object.h"
#include "Camera.h"
#include "RenderState.h"

namespace Mimema {
    class Level {
        static const std::string levelFolderLocation;

    public:
        glm::vec3 bbPos = {0, 0, 0};
        glm::vec3 bbSize = {0, 0, 0};

        glm::vec4 backgroundColor = glm::vec4(0, 0, 0, 1);

        glm::vec3 ambientLightColor = {0, 0, 0};
        float ambientLightIntensity = 0.0f;

        Camera camera = Camera(Camera::CameraState());

        std::list<DirectionalLight> directionalLights;
        std::list<PointLight> pointLights;
        std::list<Object> objects;

        RenderState getState();

        Level() = default;
        explicit Level(const std::string& levelFile, const std::map<std::string, Renderable*>& renderables);
    };
}

#endif // MIMEMA_LEVEL_H