#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp>
#include <glm/ext.hpp>
#include <glm/geometric.hpp>
#include <vector>
#include <set>
using namespace std;
// todo combine with model?

// todo whiteboard OOP
// todo override copy assignment for this and any other vao/vbo
struct Renderable {
    vector<glm::vec3> vertices;
    vector<int> faces;
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
    set<glm::vec3, cmp> normals;
    set<glm::vec3, cmp> edges;

    unsigned int VAO, VBO;

    void generateVAO();

    void calculateNormalsAndEdges();

    ~Renderable();

    // todo watch out for default constructors when destructor will ruin stuff, fix later
    Renderable() = default;
    bool setVAOVBO = false;

    Renderable(glm::vec3 position, glm::vec3 size);

private:
    float* generateVBOVertexArray();
};