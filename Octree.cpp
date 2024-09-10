#include "Octree.h"

Mimema::Octree::OctreeNode::OctreeNode(const glm::vec3& bbPos, const glm::vec3& bbSize) {
    this->bbPos = bbPos;
    this->bbSize = bbSize;
}

Mimema::Octree::Octree(const glm::vec3& position, const glm::vec3& size, int bucketMax, int divisionMax) : root(position, size) {
    this->bucketMax = bucketMax;
    this->divisionMax = divisionMax;
}

void Mimema::Octree::levelOrder() {
    std::queue<OctreeNode*> nodes;
    nodes.push(&root);

    std::cout << "Level Order of Octree with BucketMax: " << bucketMax << " and DivisionMax: " << divisionMax << std::endl;

    int level = 0;
    while (!nodes.empty()) {
        std::cout << "Level " << level++ << ":" << std::endl;
        int qSize = nodes.size();

        while (qSize-- > 0) {
            OctreeNode* curr = nodes.front();

            if (!curr->objects.empty()) {
                std::cout << curr->objects.size() << " ";
            }

            for (int i = 0; i < curr->childNodes.size(); i++) {
                OctreeNode* child = &curr->childNodes[i];
                nodes.push(child);
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


void Mimema::Octree::addObject(const Object* object) {
    if (object == nullptr) {
        return;
    }

    const Collider& collider = object->getCollider();

    std::queue<OctreeNode*> nodes;
    nodes.push(&root);

    int divisions = 0;
    while (!nodes.empty()) {
        int qSize = nodes.size();

        while (qSize-- > 0) {
            OctreeNode& currNode = *nodes.front();
            nodes.pop();

            if (!collider.checkBoundingBoxIntersection(currNode.bbPos, currNode.bbSize)) {
                continue;
            }

            if (currNode.childNodes.empty()) {
                currNode.objects.push_back(object);
            }
            else {
                for (int i = 0; i < currNode.childNodes.size(); i++) {
                    nodes.push(&currNode.childNodes[i]);
                }
            }

            bool divisionLimitHit = divisionMax != -1 && divisions >= divisionMax;
            bool bucketLimitHit = bucketMax != -1 && currNode.objects.size() >= bucketMax;
            if (!divisionLimitHit && bucketLimitHit) {
                for (int i = 0; i < 8; i++) {
                    glm::vec3 bbSize = currNode.bbSize / 2.0f;
                    glm::vec3 bbPos;

                    switch (i) {
                        case 0:
                            bbPos.x = currNode.bbPos.x;
                            bbPos.y = currNode.bbPos.y;
                            bbPos.z = currNode.bbPos.z;
                            break;
                        case 1:
                            bbPos.x = currNode.bbPos.x + bbSize.x;
                            bbPos.y = currNode.bbPos.y;
                            bbPos.z = currNode.bbPos.z;
                            break;
                        case 2:
                            bbPos.x = currNode.bbPos.x;
                            bbPos.y = currNode.bbPos.y + bbSize.y;
                            bbPos.z = currNode.bbPos.z;
                            break;
                        case 3:
                            bbPos.x = currNode.bbPos.x;
                            bbPos.y = currNode.bbPos.y;
                            bbPos.z = currNode.bbPos.z + bbSize.z;
                            break;
                        case 4:
                            bbPos.x = currNode.bbPos.x + bbSize.x;
                            bbPos.y = currNode.bbPos.y + bbSize.y;
                            bbPos.z = currNode.bbPos.z;
                            break;
                        case 5:
                            bbPos.x = currNode.bbPos.x + bbSize.x;
                            bbPos.y = currNode.bbPos.y;
                            bbPos.z = currNode.bbPos.z + bbSize.z;
                            break;
                        case 6:
                            bbPos.x = currNode.bbPos.x;
                            bbPos.y = currNode.bbPos.y + bbSize.y;
                            bbPos.z = currNode.bbPos.z + bbSize.z;
                            break;
                        case 7:
                            bbPos.x = currNode.bbPos.x + bbSize.x;
                            bbPos.y = currNode.bbPos.y + bbSize.y;
                            bbPos.z = currNode.bbPos.z + bbSize.z;
                            break;
                        default:
                            break;
                    }
                    OctreeNode child = OctreeNode(bbPos, bbSize);
                    currNode.childNodes.at(i) = child;

                    for (int i = 0; i < currNode.objects.size(); i++) {
                        const Collider& currCollider = currNode.objects[i]->getCollider();

                        if (currCollider.checkBoundingBoxIntersection(child.bbPos, child.bbSize)) {
                            child.objects.push_back(currNode.objects[i]);
                        }
                    }
                }
                currNode.objects.clear();
            }
        }

        divisions++;
    }
}

glm::vec3 Mimema::Octree::checkCollision(const Object* object) {
    if (object == nullptr) {
        return {0, 0, 0};
    }

    const Collider& collider = object->getCollider();

    std::queue<OctreeNode*> nodes;
    nodes.push(&root);

    while (!nodes.empty()) {
        int qSize = nodes.size();

        while (qSize-- > 0) {
            OctreeNode& currNode = *nodes.front();
            nodes.pop();

            if (!collider.checkBoundingBoxIntersection(currNode.bbPos, currNode.bbSize)) {
                continue;
            }

            for (int i = 0; i < currNode.objects.size(); i++) {
                if (object == currNode.objects[i]) {
                    continue;
                }

                const Collider& rhs = currNode.objects[i]->getCollider();

                glm::vec3 collisionVector = collider.checkSatIntersection(rhs);

                if (collisionVector != glm::vec3(0, 0, 0)) {
                    return collisionVector;
                }
            }

            for (int i = 0; i < currNode.childNodes.size(); i++) {
                nodes.push(&currNode.childNodes[i]);
            }
        }
    }

    return {0, 0, 0};
}