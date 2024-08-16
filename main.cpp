#include "EngineCore.h"
using namespace std;

int main() {
    // TODO valgrind this project for mem leaks (supported in clion)

    // Next Todos:
    // Clean up Broadphase so isn't lagging
    // do spotlight lighting instead of directional (looks way better with physics with spotlight)
    // clean up code (various todos)

    // Temporary
    EngineCore engine;
    engine.engineLoop(600, 600, "Mimema");
    return 0;
}
