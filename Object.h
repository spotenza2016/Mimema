#ifndef MIMEMA_OBJECT_H
#define MIMEMA_OBJECT_H

#include <string>
#include <vector>

#include "tracy/Tracy.hpp"

#include "Renderable.h"
#include "Collider.h"

namespace Mimema {
    class Object {
    public:
        struct ObjectState {
            glm::vec3 translate = glm::vec3(0, 0, 0);
            glm::vec3 angles = glm::vec3(0, 0, 0);
            glm::vec3 scale = glm::vec3(1, 1, 1);

            ObjectState operator*(float alpha) const;

            ObjectState operator+(const ObjectState& rhs) const;
        };

    private:
        ObjectState objectState;

        Collider collider;

        void updateCollider();

    public:
        uint32_t physicsMask = 0xFFFFFFFF;
        uint32_t collisionMask = 0xFFFFFFFF;

        glm::vec3 velocity = glm::vec3(0, 0, 0);
        glm::vec3 acceleration = glm::vec3(0, 0, 0);

        void passTime(float deltaT);
        void collisionOccurred();

        const Renderable* model = nullptr;

        explicit Object(Mimema::Renderable* model);

        // Getters
        const glm::vec3& getTranslate();
        const glm::vec3& getAngles();
        const glm::vec3& getScale();
        const Collider& getCollider() const;
        const ObjectState& getObjectState();

        // Setters
        void setTranslate(const glm::vec3& translate);
        void addToTranslate(const glm::vec3& add);
        void setAngles(const glm::vec3& angles);
        void addToAngles(const glm::vec3& add);
        void setScale(const glm::vec3& scale);
        void addToScale(const glm::vec3& add);
        void setModelMatrix(const glm::vec3& translate, const glm::vec3& angles, const glm::vec3& scale);
        void addToModelMatrix(const glm::vec3& addTranslate, const glm::vec3& addAngles, const glm::vec3& addScale);
    };
}

#endif