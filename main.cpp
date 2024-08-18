#include "EngineCore.h"
using namespace std;

int main() {
    // TODO valgrind this project for mem leaks (supported in clion) (maybe other time profiler also?)
        // setup memory in profiler for leaks (needs to override the thingy), and prolly also build its exe in it

    // Next Todos:
    // fix matrix thing
        // add attenuation and spotlight later
    // clean up code (various todos)
    // waaay later, maybe custom allocator?
    // formatting automatically? for workflow?


    // Temporary
    EngineCore engine;
    engine.engineLoop(600, 600, "Mimema");
    return 0;
}
