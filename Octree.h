#pragma once
#include <iostream>
#include <queue>
#include <cmath>
#include "Object.h"
#include "PhysicsObject.h"
#include "tracy/Tracy.hpp"
#include "CollisionObject.h"

using namespace std;

// todo avoid delete/new if going to be same size?
class Octree {
private:
    struct Node {
        vector<Node*> children;
        vector<Object*> contains;

        // todo should have this position/region only within collision bounds so actually splits
        glm::vec3 position;
        glm::vec3 regionSize;
        // todo make sure this isn't misused, probably rename to count or below, hate that regionSize isn't size
        int size;
        bool leaf;
        Node(glm::vec3 position, glm::vec3 size) {
            this->position = position;
            this->regionSize = size;
            children.resize(8);
            this->size = 0;
        }

        bool intersects(const CollisionObject& collision);
    };
    Node* root = nullptr;

    // todo optimize settings?
    int bucketMax = 16;
    int divisionMax = 30;
    int size = 0;
    void addObjectHelper(Node *currNode, Object *object, int divisions);
    void addToLeaf(Node* currNode, Object *object, int divisions);
    bool collisionHelper(Node* currNode, PhysicsObject *object) const;

public:
    void addObject(Object *object);
    bool collisionCheck(PhysicsObject* object) const;

    Octree(const glm::vec3& position, const glm::vec3& size);

    ~Octree();
    void destructorHelper(Node* currNode);

    void levelOrder();
};