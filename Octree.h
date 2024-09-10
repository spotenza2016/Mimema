#ifndef MIMEMA_OCTREE_H
#define MIMEMA_OCTREE_H

#include <queue>
#include <iostream>

#include "tracy/Tracy.hpp"

#include "Object.h"

namespace Mimema {
    class Octree {
    private:
        struct OctreeNode {
            glm::vec3 bbPos;
            glm::vec3 bbSize;

            std::vector<OctreeNode> childNodes;
            std::vector<const Object*> objects;

            OctreeNode(const glm::vec3& bbPos, const glm::vec3& bbSize);
        };

        OctreeNode root;

        // -1 means unlimited
        int bucketMax = -1;
        int divisionMax = -1;

    public:
        Octree(const glm::vec3& position, const glm::vec3& size, int bucketMax = -1, int divisionMax = -1);

        void levelOrder();
        void addObject(const Object* object);
        glm::vec3 checkCollision(const Object* object);
    };
}

#endif // MIMEMA_OCTREE_H