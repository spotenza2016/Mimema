#include "EngineCore.h"
using namespace std;

int main() {
    // TODO valgrind this project for mem leaks (supported in clion)

    // Temporary
    EngineCore engine;
    engine.engineLoop(600, 600, "Mimema");
    return 0;
}
