#include "Core.h"
using namespace std;

int main() {
    Mimema::Core::instance().init(1080, 1080, "Mimema");
    Mimema::Core::instance().loadLevel("testLevel.MIMEMA_LEVEL");
    Mimema::Core::instance().engineLoop();
    Mimema::Core::instance().shutdown();
    return 0;
}
