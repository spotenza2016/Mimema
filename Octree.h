#pragma once
#include <iostream>
#include <queue>
#include <cmath>
#include "Object.h"
#include "CollisionBox.h"
using namespace std;

class Octree {
private:
    struct Node {
        vector<Node*> children;
        vector<Object*> contains;
        CollisionBox region = CollisionBox(glm::vec3(), glm::vec3());
        int size;
        bool leaf;
        Node(glm::vec3 position, glm::vec3 size) {
            region = CollisionBox(position, size);
            children.resize(8);
            this->size = 0;
        }
    };
    Node* root = nullptr;
    int bucketMax = 2;
    int divisionMax = 100000;
    int size = 0;
    void addObjectHelper(Node *currNode, Object *object, int divisions);
    void addToLeaf(Node* currNode, Object *object, int divisions);
    bool collisionHelper(Node* currNode, Object *object) const;

public:
    void addObject(Object *object);
    bool collisionCheck(Object* object) const;

    Octree(const CollisionBox& box);

    ~Octree();
    void destructorHelper(Node* currNode);

    void levelOrder();
};