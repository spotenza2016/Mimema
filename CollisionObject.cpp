#include "CollisionObject.h"
using namespace std;

pair<double, double> CollisionObject::project(const CollisionObject& box, const glm::vec3& axis) {
    const vector<glm::vec3>& vertices = box.vertices;
    double min, max;
    for (int i = 0; i < vertices.size(); i++) {
        // todo figure out better way so not multiplying so many times
        glm::vec3 currVertex = vertices[i];
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
    ZoneScopedN("SAT");
    // todo make into function
    float xIntersection = min(bbPos.x + bbSize.x, rhs.bbPos.x + rhs.bbSize.x) - max(bbPos.x, rhs.bbPos.x);
    float yIntersection = min(bbPos.y + bbSize.y, rhs.bbPos.y + rhs.bbSize.y) - max(bbPos.y, rhs.bbPos.y);
    float zIntersection = min(bbPos.z + bbSize.z, rhs.bbPos.z + rhs.bbSize.z) - max(bbPos.z, rhs.bbPos.z);

    if (!(xIntersection >= 0 && yIntersection >= 0 && zIntersection >= 0)) {
        return {false, {}};
    }

    // todo not handling zero width correctly
    set<glm::vec3, cmp> axes;

    findAxes(*this, rhs, axes);

    if (axes.size() == 0) {
        return {false, {}};
    }

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

// todo may want to support pills/spheres
void CollisionObject::findAxes(const CollisionObject& lhs, const CollisionObject& rhs, set<glm::vec3, cmp>& axes) {
    // Face Normals
    // todo avoid the matrix multiplication multiple times
    // todo maybe precalculate these normals?
    for (int i = 0; i < lhs.normals.size(); i++) {
        axes.insert(lhs.normals[i]);
    }
    for (int i = 0; i < rhs.normals.size(); i++) {
        axes.insert(rhs.normals[i]);
    }

    // Edge Pairs
    for (int i = 0; i < lhs.edges.size(); i++) {
        for (int j = 0; j < rhs.edges.size(); j++) {
            glm::vec3 axis = glm::cross(lhs.edges[i], rhs.edges[j]);
            if (axis == glm::vec3(0)) {
                continue;
            }
            axes.insert(glm::normalize(axis));
        }
    }
}

CollisionObject::CollisionObject(const Renderable& renderable, glm::mat4 matrix) {
    ZoneScoped;
    glm::vec3 minimum;
    glm::vec3 maximum;

    // todo avoid all new calls when I can, shouldn't ever be using new every frame if i can avoid
    vertices.resize(renderable.vertices.size());
    for (int i = 0; i < renderable.vertices.size(); i++) {
        ZoneScopedN("Vertices");
        vertices[i] = matrix * glm::vec4(renderable.vertices[i], 1);

        if (i == 0) {
            minimum = vertices[i];
            maximum = vertices[i];
        } else {
            ZoneScopedN("Check");
            if (vertices[i].x < minimum.x) {
                minimum.x = vertices[i].x;
            }
            if (vertices[i].y < minimum.y) {
                minimum.y = vertices[i].y;
            }
            if (vertices[i].z < minimum.z) {
                minimum.z = vertices[i].z;
            }
            if (vertices[i].x > maximum.x) {
                maximum.x = vertices[i].x;
            }
            if (vertices[i].y > maximum.y) {
                maximum.y = vertices[i].y;
            }
            if (vertices[i].z > maximum.z) {
                maximum.z = vertices[i].z;
            }
        }
    }

    bbPos = minimum;
    bbSize = maximum - minimum;

    faces = renderable.faces;

    //todo may want to setup a test map for SAT? cool demo
    normals.resize(renderable.normals.size());
    auto iter = renderable.normals.begin();
    for (int i = 0; i < renderable.normals.size(); i++) {
        // todo should officialize all these
        ZoneScopedN("Normals");
        normals[i] = glm::normalize(matrix * glm::vec4(*iter, 0));
        iter++;
    }

    edges.resize(renderable.edges.size());
    iter = renderable.edges.begin();
    for (int i = 0; i < renderable.edges.size(); i++) {
        ZoneScopedN("Edges");
        edges[i] = matrix * glm::vec4(*iter, 0);
        iter++;
    }
}

