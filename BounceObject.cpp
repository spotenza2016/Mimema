#include "BounceObject.h"

void BounceObject::collisionOccurred() {
    // todo fix
    this->velocity = velocity * -1.0f;
}
