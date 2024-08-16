#include "CollisionObject.h"
using namespace std;

pair<double, double> CollisionObject::project(const CollisionObject& box, const glm::vec3& axis) {
    const vector<glm::vec3>& vertices = box.vertices;
    double min, max;
    for (int i = 0; i < vertices.size(); i++) {
        // todo figure out better way so not multiplying so many times
        glm::vec3 currVertex = glm::vec3(box.matrix * glm::vec4(vertices[i], 1));
        double curr = glm::dot(currVertex, axis);
        if (i == 0) {
            min = curr;
            max = curr;
        }
        else {
            if (curr < min) {
                min = curr;
            }
            if (curr > max) {
                max = curr;
            }
        }
    }
    return {min, max};
}

pair<bool, pair<double, glm::vec3>> CollisionObject::intersects(const CollisionObject& rhs) {
    // todo not handling zero width correctly
    set<glm::vec3, cmp> axes;

    findAxes(*this, rhs, axes);

    // todo make cleaner
    bool overlapSet = false;
    double overlap;
    glm::vec3 smallestAxis;
    for (auto iter = axes.begin(); iter != axes.end(); iter++) {
        glm::vec3 axis = *iter;
        pair<double, double> projectionLhs = project(*this, axis);
        pair<double, double> projectionRhs = project(rhs, axis);
        double currOverlap = min(projectionLhs.second, projectionRhs.second) - max(projectionLhs.first, projectionRhs.first);

        // todo something is seriously up with this,
        // todo don't forget to set plane back to plane
        if (currOverlap < 0) {
            return {false, {}};
        }

        if ((projectionLhs.first > projectionRhs.first && projectionLhs.second < projectionRhs.second) || (projectionLhs.first < projectionRhs.first && projectionLhs.second > projectionRhs.second)) {
            double minDist = max(projectionLhs.first, projectionRhs.first) - min(projectionLhs.first, projectionRhs.first);
            double maxDist = max(projectionLhs.second, projectionRhs.second) - min(projectionLhs.second, projectionRhs.second);

            if (maxDist < minDist) {
                currOverlap += maxDist;
                axis *= -1;
            }
            else {
                currOverlap += minDist;
            }
        }

        if (!overlapSet || currOverlap < overlap) {
            overlap = currOverlap;
            smallestAxis = axis;
            overlapSet = true;
        }
    }

    return {true, {overlap, smallestAxis}};
}

void CollisionObject::findAxes(const CollisionObject& lhs, const CollisionObject& rhs, set<glm::vec3, cmp> &axes) {
    // Face Normals
    // todo maybe precalculate these normals?
    for (int i = 0; i < lhs.faces.size(); i += 3) {
        glm::vec3 vecOne = lhs.vertices[lhs.faces[i + 1] - 1] - lhs.vertices[lhs.faces[i] - 1];
        vecOne = lhs.matrix * glm::vec4(vecOne, 0);
        glm::vec3 vecTwo = lhs.vertices[lhs.faces[i + 2] - 1] - lhs.vertices[lhs.faces[i] - 1];
        vecTwo = lhs.matrix * glm::vec4(vecTwo, 0);
        axes.insert(glm::normalize(glm::cross(vecOne, vecTwo)));
    }
    for (int i = 0; i < rhs.faces.size(); i += 3) {
        glm::vec3 vecOne = rhs.vertices[rhs.faces[i + 1] - 1] - rhs.vertices[rhs.faces[i] - 1];
        vecOne = rhs.matrix * glm::vec4(vecOne, 0);
        glm::vec3 vecTwo = rhs.vertices[rhs.faces[i + 2] - 1] - rhs.vertices[rhs.faces[i] - 1];
        vecTwo = rhs.matrix * glm::vec4(vecTwo, 0);
        axes.insert(glm::normalize(glm::cross(vecOne, vecTwo)));
    }

    // Edge Pairs
    for (int i = 0; i < lhs.faces.size(); i += 3) {
        for (int j = 0; j < 3; j++) {
            glm::vec3 currLhsEdge = lhs.vertices[lhs.faces[i + ((j + 1) % 3)] - 1] - lhs.vertices[lhs.faces[i + j] - 1];
            currLhsEdge = lhs.matrix * glm::vec4(currLhsEdge, 0);
            for (int k = 0; k < rhs.faces.size(); k += 3) {
                for (int l = 0; l < 3; l++) {
                    glm::vec3 currRhsEdge = rhs.vertices[rhs.faces[k + ((l + 1) % 3)] - 1] - rhs.vertices[rhs.faces[k + l] - 1];
                    currRhsEdge = rhs.matrix * glm::vec4(currRhsEdge, 0);
                    axes.insert(glm::normalize(glm::cross(currLhsEdge, currRhsEdge)));
                }
            }
        }
    }
}

float* CollisionObject::generateVBOVertexArray() {
    // Generate Vertices Array
    // todo maybe rework faces?
    float* vertexArray = new float[faces.size() * 4];
    for (int i = 0; i < faces.size(); i++) {
        int vboIndex = 4 * i;
        vertexArray[vboIndex] = vertices[faces[i] - 1].x;
        vertexArray[vboIndex + 1] = vertices[faces[i] - 1].y;
        vertexArray[vboIndex + 2] = vertices[faces[i] - 1].z;
        vertexArray[vboIndex + 3] = 1;
    }

    return vertexArray;
}

//todo may want to transfer all these to opengl handler
void CollisionObject::generateVAO() {
    float* vertexArray = generateVBOVertexArray();

    // Create VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Create VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, faces.size() * 4 * sizeof(float), vertexArray, GL_STATIC_DRAW);

    // Vertices for VBO
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    delete vertexArray;

    vaoVboSet = true;
}

CollisionObject::~CollisionObject() {
    // todo better protection on deleting bad VAO/VBO
    // todo can I somehow prevent from creating temporaries? maybe override the copy assignment?
    // Delete OpenGL Buffers
    if (vaoVboSet) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
}
