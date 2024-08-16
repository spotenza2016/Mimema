#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp>
#include <glm/ext.hpp>
#include <glm/geometric.hpp>
#include <glm/mat4x4.hpp>
#include <algorithm>
#include <vector>
#include <iostream>
#include <set>
using namespace std;

// todo convert this to collision shape, honestly should be ez
struct CollisionObject {
    vector<glm::vec3> vertices;
    vector<int> faces;
    glm::mat4 matrix = glm::mat4(1);

    pair<bool, pair<double, glm::vec3>> intersects(const CollisionObject& rhs);

    unsigned int VAO, VBO;
    bool vaoVboSet = false;

    ~CollisionObject();

    // todo should do better method for this since might be uninitialized
    void generateVAO();

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

    float* generateVBOVertexArray();

    static pair<double, double> project(const CollisionObject& box, const glm::vec3& axis);

    static void findAxes(const CollisionObject& lhs, const CollisionObject& rhs, set<glm::vec3, cmp>& axes);
};