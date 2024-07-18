#include "Octree.h"

void Octree::addObject(Object *object) {
    size++;
    addObjectHelper(root, object, 0);
}

void Octree::addObjectHelper(Node* currNode, Object* object, int divisions) {
    CollisionBox box = object->getCollision();
    if (!currNode->leaf) {
        for (int i = 0; i < currNode->children.size(); i++) {
            Node* child = currNode->children.at(i);

            if (child->region.intersects(box)) {
                addObjectHelper(child, object, divisions + 1);
            }
        }
    } else {
        addToLeaf(currNode, object, divisions);
    }
}

bool Octree::collisionHelper(Node* currNode, Object *object) const {
    if (!currNode->leaf) {
        for (int i = 0; i < currNode->children.size(); i++) {
            Node* child = currNode->children.at(i);

            if (child->region.intersects(object->getCollision())) {
                if (collisionHelper(child, object)) {
                    return true;
                }
            }
        }
        return false;
    } else {
        for (int i = 0; i < currNode->size; i++) {
            Object* result = currNode->contains.at(i);
            CollisionBox resultBox = result->getCollision();
            if (result != object && resultBox.intersects(object->getCollision())) {
                return true;
            }
        }
        return false;
    }
}

bool Octree::collisionCheck(Object* object) const {
    if (root == nullptr) {
        return false;
    }
    else if (root->region.intersects(object->getCollision())) {
        return collisionHelper(root, object);
    }
    return false;
}

Octree::Octree(const CollisionBox& box) {
    Node* node = new Node(box.position, box.size);
    root = node;
    node->leaf = true;
}

Octree::~Octree() {
    destructorHelper(root);
}

void Octree::destructorHelper(Octree::Node *currNode) {
    if (currNode != nullptr) {
        if (!currNode->leaf) {
            for (int i = 0; i < currNode->children.size(); i++) {
                destructorHelper(currNode->children.at(i));
            }
        }
        delete currNode;
    }
}

void Octree::levelOrder() {
    if (root == nullptr) {
        return;
    }
    queue<Node*> nodes;
    nodes.push(root);
    int level = 0;
    cout << size << " size with division of " << divisionMax << endl;
    cout << "Level Order:" << endl;
    while (!nodes.empty()) {
        cout << "Level " << level++ << ":" << endl;
        int size = nodes.size();
        for (int i = 0; i < size; i++) {
            Node* curr = nodes.front();

            cout << curr->size << " ";

            for (int j = 0; j < curr->children.size(); j++) {
                Node* child = curr->children.at(j);
                if (child != nullptr) {
                    nodes.push(curr->children.at(j));
                }
            }
            nodes.pop();
        }
        cout << endl;
    }
}

void Octree::addToLeaf(Octree::Node* currNode, Object* object, int divisions) {
    currNode->size++;
    currNode->contains.push_back(object);
    if (divisions < divisionMax && currNode->size > bucketMax) {
        currNode->leaf = false;
        for (int i = 0; i < 8; i++) {
            glm::vec3 size = currNode->region.size / 2.0f;
            glm::vec3 position;

            switch (i) {
                case 0:
                    position.x = currNode->region.position.x;
                    position.y = currNode->region.position.y;
                    position.z = currNode->region.position.z;
                    break;
                case 1:
                    position.x = currNode->region.position.x + size.x;
                    position.y = currNode->region.position.y;
                    position.z = currNode->region.position.z;
                    break;
                case 2:
                    position.x = currNode->region.position.x;
                    position.y = currNode->region.position.y + size.y;
                    position.z = currNode->region.position.z;
                    break;
                case 3:
                    position.x = currNode->region.position.x;
                    position.y = currNode->region.position.y;
                    position.z = currNode->region.position.z + size.z;
                    break;
                case 4:
                    position.x = currNode->region.position.x + size.x;
                    position.y = currNode->region.position.y + size.y;
                    position.z = currNode->region.position.z;
                    break;
                case 5:
                    position.x = currNode->region.position.x + size.x;
                    position.y = currNode->region.position.y;
                    position.z = currNode->region.position.z + size.z;
                    break;
                case 6:
                    position.x = currNode->region.position.x;
                    position.y = currNode->region.position.y + size.y;
                    position.z = currNode->region.position.z + size.z;
                    break;
                case 7:
                    position.x = currNode->region.position.x + size.x;
                    position.y = currNode->region.position.y + size.y;
                    position.z = currNode->region.position.z + size.z;
                    break;
                default:
                    break;
            }
            Node* child = new Node(position, size);
            currNode->children.at(i) = child;
            child->leaf = true;

            for (int i = 0; i < currNode->size; i++) {
                if (currNode->contains.at(i)->getCollision().intersects(child->region)) {
                    child->contains.push_back(currNode->contains.at(i));
                    child->size++;
                }
            }
        }
        currNode->size = 0;
        currNode->contains.clear();
    }
}