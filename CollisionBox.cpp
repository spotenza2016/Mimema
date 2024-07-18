#include "CollisionBox.h"
using namespace std;

bool CollisionBox::intersects(const CollisionBox& rhs) {
    int xIntersection = min(position.x + size.x, rhs.position.x + rhs.size.x) - max(position.x, rhs.position.x);
    int yIntersection = min(position.y + size.y, rhs.position.y + rhs.size.y) - max(position.y, rhs.position.y);
    int zIntersection = min(position.z + size.z, rhs.position.z + rhs.size.z) - max(position.z, rhs.position.z);

    return xIntersection >= 0 && yIntersection >= 0 && zIntersection >= 0;
}
