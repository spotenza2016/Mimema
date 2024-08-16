#include "EngineCore.h"
using namespace std;

int main() {
    // TODO valgrind this project for mem leaks (supported in clion) (maybe other time profiler also?)

    // Next Todos:
    // Clean up Broadphase so isn't lagging
    // do point lighting instead of directional (looks way better with physics with spotlight)
        // add attenuation and spotlight later
    // clean up code (various todos)

    // Temporary
    EngineCore engine;
    engine.engineLoop(1920, 1080, "Mimema");
    return 0;
}
