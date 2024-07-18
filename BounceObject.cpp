#include "BounceObject.h"

void BounceObject::collisionOccurred() {
    this->velocity = velocity * -1.0f;
}
