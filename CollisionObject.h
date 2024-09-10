#pragma once
#include <glm/vec3.hpp>
#include <glm/ext.hpp>
#include <glm/geometric.hpp>
#include <glm/mat4x4.hpp>
#include <algorithm>
#include <vector>
#include <iostream>
#include <set>
#include "Renderable.h"
#include "tracy/Tracy.hpp"
using namespace std;

// todo convert this to collision shape, honestly should be ez
struct CollisionObject {
    vector<glm::vec3> vertices;
    vector<int> faces;

    // todo fix these
    CollisionObject() = default;
    vector<glm::vec3> normals;
    vector<glm::vec3> edges;

    glm::vec3 bbPos;
    glm::vec3 bbSize;

    CollisionObject(const Renderable& renderable, glm::mat4 matrix);

    pair<bool, pair<double, glm::vec3>> intersects(const CollisionObject& rhs);

private:
    struct cmp {
        // todo this may not be the best, turning the final one to true results in major lag for some reason (due to set being obscenely big)
        // may need to research how sets function
        bool operator()(const glm::vec3& lhs, const glm::vec3& rhs) {
            if (lhs.x < rhs.x) {
                return true;
            }
            else if (lhs.x > rhs.x) {
                return false;
            }
            else if (lhs.y < rhs.y) {
                return true;
            }
            else if (lhs.y > rhs.y) {
                return false;
            }
            else if (lhs.z < rhs.z) {
                return true;
            }
            else if (lhs.z > rhs.z) {
                return true;
            }
            else {
                return false;
            }
        }
    };

    static pair<double, double> project(const CollisionObject& box, const glm::vec3& axis);

    static void findAxes(const CollisionObject& lhs, const CollisionObject& rhs, set<glm::vec3, cmp>& axes);
};