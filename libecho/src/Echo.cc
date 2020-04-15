#include "Echo.h"
#include "QTInitializer.h"

void Echo::initEcho(int a_argc, char **a_argv) {
    static QTInitializer init{a_argc, a_argv};
}