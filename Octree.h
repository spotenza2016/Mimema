#pragma once
#include <iostream>
#include <queue>
#include <cmath>
#include "Object.h"
#include "PhysicsObject.h"
#include "CollisionObject.h"
#include "CollisionBox.h"

using namespace std;

class Octree {
private:
    struct Node {
        vector<Node*> children;
        vector<Object*> contains;
        // Major TODO! Need to convert this to not use SAT for collision since it's so costly
        CollisionBox region = CollisionBox({}, {});
        int size;
        bool leaf;
        Node(glm::vec3 position, glm::vec3 size) {
            region = CollisionBox(position, size);
            children.resize(8);
            this->size = 0;
        }
    };
    Node* root = nullptr;
    int bucketMax = 8;
    int divisionMax = 100000;
    int size = 0;
    void addObjectHelper(Node *currNode, Object *object, int divisions);
    void addToLeaf(Node* currNode, Object *object, int divisions);
    bool collisionHelper(Node* currNode, PhysicsObject *object) const;

public:
    void addObject(Object *object);
    bool collisionCheck(PhysicsObject* object) const;

    Octree(const CollisionBox& box);

    ~Octree();
    void destructorHelper(Node* currNode);

    void levelOrder();
};