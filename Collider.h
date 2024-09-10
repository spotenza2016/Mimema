#ifndef MIMEMA_COLLIDER_H
#define MIMEMA_COLLIDER_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp>
#include <glm/ext.hpp>
#include <glm/geometric.hpp>
#include <glm/mat4x4.hpp>
#include <vector>
#include <iostream>
#include <set>

#include "tracy/Tracy.hpp"

namespace Mimema {
    class Collider {
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec3> edges;

        // Bounding Box
        glm::vec3 bbPos;
        glm::vec3 bbSize;

        // Custom comparator for vec3 for use in vec3 set
        struct cmpVec3 {
            bool operator()(const glm::vec3& lhs, const glm::vec3& rhs);
        };

        // Returns the {minimum, maximum} when projected onto the given axis
        std::pair<double, double> project(const glm::vec3& axis) const;

        // Finds all axes to check for SAT
        void findAxes(const Collider& rhs, std::set<glm::vec3, cmpVec3>& axes) const;

    public:
        Collider();

        // Make a cubic collider
        Collider(const glm::vec3& bbPos, const glm::vec3& bbSize, const glm::mat4& matrix);

        bool checkBoundingBoxIntersection(const glm::vec3& rhsPos, const glm::vec3& rhsSize) const;

        glm::vec3 checkSatIntersection(const Collider& rhs) const;
    };
}

#endif // MIMEMA_COLLIDER_H