#include "Collider.h"

bool Mimema::Collider::cmpVec3::operator()(const glm::vec3& lhs, const glm::vec3& rhs) {
    if (lhs.x != rhs.x) {
        return lhs.x < rhs.x;
    }
    if (lhs.y != rhs.y) {
        return lhs.y < rhs.y;
    }
    if (lhs.z != rhs.z) {
        return lhs.z < rhs.z;
    }
    return false;
}

std::pair<double, double> Mimema::Collider::project(const glm::vec3& axis) const {
    double min = INT_MAX;
    double max = INT_MIN;

    for (int i = 0; i < vertices.size(); i++) {
        const glm::vec3& currVertex = vertices[i];

        double position = glm::dot(currVertex, axis);

        if (position < min) {
            min = position;
        }
        if (position > max) {
            max = position;
        }
    }

    return {min, max};
}

void Mimema::Collider::findAxes(const Collider& rhs, std::set<glm::vec3, cmpVec3>& axes) const {
    // Face Normals
    for (auto iter = normals.begin(); iter != normals.end(); iter++) {
        axes.insert(*iter);
    }
    for (auto iter = rhs.normals.begin(); iter != rhs.normals.end(); iter++) {
        axes.insert(*iter);
    }

    // Edge Pairs
    for (auto iterLhs = edges.begin(); iterLhs != edges.end(); iterLhs++) {
        for (auto iterRhs = rhs.edges.begin(); iterRhs != rhs.edges.end(); iterRhs++) {
            glm::vec3 axis = glm::cross(*iterLhs, *iterRhs);
            if (axis == glm::vec3(0, 0, 0)) {
                continue;
            }
            axes.insert(glm::normalize(axis));
        }
    }
}

Mimema::Collider::Collider() {
    bbPos = glm::vec3(0);
    bbSize = glm::vec3(0);
}

Mimema::Collider::Collider(const glm::vec3& bbPos, const glm::vec3& bbSize, const glm::mat4& matrix) {
    vertices = {bbPos,
                bbPos + glm::vec3(bbSize.x, 0, 0),
                bbPos + glm::vec3(0, 0, bbSize.z),
                bbPos + glm::vec3(bbSize.x, 0, bbSize.z),
                bbPos + glm::vec3(0, bbSize.y, 0),
                bbPos + glm::vec3(bbSize.x, bbSize.y, 0),
                bbPos + glm::vec3(0,bbSize.y, bbSize.z),
                bbPos + bbSize};

    glm::vec3 minimum;
    glm::vec3 maximum;
    for (int i = 0; i < vertices.size(); i++) {
        vertices[i] = matrix * glm::vec4(vertices[i], 1);

        if (i == 0) {
            minimum = vertices[i];
            maximum = vertices[i];
            continue;
        }

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

    this->bbPos = minimum;
    this->bbSize = maximum - minimum;

    std::vector<glm::vec<3, int>> vertexIndices = {{0, 2, 1},
                                                   {1, 2, 3},
                                                   {0, 1, 4},
                                                   {1, 5, 4},
                                                   {2, 6, 3},
                                                   {3, 6, 7},
                                                   {0, 4, 2},
                                                   {2, 4, 6},
                                                   {1, 3, 5},
                                                   {3, 7, 5},
                                                   {4, 5, 6},
                                                   {5, 7, 6}};

    // Set of all normals and edges
    std::set<glm::vec3, cmpVec3> normalSet;
    std::set<glm::vec3, cmpVec3> edgeSet;
    for (int i = 0; i < vertexIndices.size(); i++) {
        glm::vec3 vecOne = vertices[vertexIndices[i].y] - vertices[vertexIndices[i].x];
        glm::vec3 vecTwo = vertices[vertexIndices[i].z] - vertices[vertexIndices[i].y];

        glm::vec3 cross = glm::cross(vecOne, vecTwo);

        if (cross != glm::vec3(0)) {
            normalSet.insert(glm::normalize(cross));
        }

        glm::vec3 currEdge = vertices[vertexIndices[i].y] - vertices[vertexIndices[i].x];
        if (currEdge != glm::vec3(0)) {
            edgeSet.insert(glm::normalize(currEdge));
        }

        currEdge = vertices[vertexIndices[i].z] - vertices[vertexIndices[i].y];
        if (currEdge != glm::vec3(0)) {
            edgeSet.insert(glm::normalize(currEdge));
        }

        currEdge = vertices[vertexIndices[i].x] - vertices[vertexIndices[i].z];
        if (currEdge != glm::vec3(0)) {
            edgeSet.insert(glm::normalize(currEdge));
        }
    }

    normals.resize(normalSet.size());
    auto iter = normalSet.begin();
    for (int i = 0; i < normalSet.size(); i++, iter++) {
        this->normals[i] = *iter;
    }
    edges.resize(edgeSet.size());
    iter = edgeSet.begin();
    for (int i = 0; i < edgeSet.size(); i++, iter++) {
        edges[i] = *iter;
    }
}

bool Mimema::Collider::checkBoundingBoxIntersection(const glm::vec3& rhsPos, const glm::vec3& rhsSize) const {
    float xIntersection = std::min(bbPos.x + bbSize.x, rhsPos.x + rhsSize.x) - std::max(bbPos.x, rhsPos.x);
    float yIntersection = std::min(bbPos.y + bbSize.y, rhsPos.y + rhsSize.y) - std::max(bbPos.y, rhsPos.y);
    float zIntersection = std::min(bbPos.z + bbSize.z, rhsPos.z + rhsSize.z) - std::max(bbPos.z, rhsPos.z);

    return xIntersection >= 0 && yIntersection >= 0 && zIntersection >= 0;
}

glm::vec3 Mimema::Collider::checkSatIntersection(const Collider& rhs) const {
    if (!checkBoundingBoxIntersection(rhs.bbPos, rhs.bbSize)) {
        return {0, 0, 0};
    }

    std::set<glm::vec3, cmpVec3> axes;

    findAxes(rhs, axes);

    if (axes.empty()) {
        return {0, 0, 0};
    }

    double overlap = 0.0f;
    glm::vec3 smallestAxis = {0, 0, 0};
    for (auto iter = axes.begin(); iter != axes.end(); iter++) {
        // Apply model matrix (faster usually to apply to axis instead of every single vertex)
        glm::vec3 axis = *iter;

        std::pair<double, double> projectionLhs = project(axis);
        std::pair<double, double> projectionRhs = rhs.project(axis);

        double currOverlap = std::min(projectionLhs.second, projectionRhs.second) - std::max(projectionLhs.first, projectionRhs.first);

        // Negative Overlap (not intersecting)
        if (currOverlap < 0) {
            return {0, 0, 0};
        }

        // Does one contain the other?
        bool rhsContainsLhs = projectionLhs.first > projectionRhs.first && projectionLhs.second < projectionRhs.second;
        bool lhsContainsRhs = projectionLhs.first < projectionRhs.first && projectionLhs.second > projectionRhs.second;

        if (rhsContainsLhs || lhsContainsRhs) {
            double lowerDist = std::max(projectionLhs.first, projectionRhs.first) - std::min(projectionLhs.first, projectionRhs.first);
            double upperDist = std::max(projectionLhs.second, projectionRhs.second) - std::min(projectionLhs.second, projectionRhs.second);

            // Define direction as contained object taking upper distance
            if (lhsContainsRhs) {
                // If rhs is inside lhs, taking upper distance involves lhs moving backwards
                axis *= -1;
            }

            // If contained, add the least amount to get out (overlap only gives the size of the contained object)
            if (upperDist < lowerDist) {
                currOverlap += upperDist;
            }
            else {
                currOverlap += lowerDist;

                // If taking lower distance, reverse
                axis *= -1;
            }
        }
            // Otherwise intersection, if rhs is farther than lhs, reverse axis since lhs needs to move backwards
        else if (projectionLhs.second < projectionRhs.second) {
            axis *= -1;
        }

        // Define zero intersection as no collision (couldn't do earlier in case was contained)
        if (currOverlap == 0) {
            return {0, 0, 0};
        }

        if (overlap == 0.0f || currOverlap < overlap) {
            overlap = currOverlap;
            smallestAxis = axis;
        }
    }

    return overlap * smallestAxis;
}