#ifndef DEMOAPP_UTILS_H
#define DEMOAPP_UTILS_H

#include <vector>

#include "Argument.h"

class Utils {
public:
    static void clear();
    static bool readArguments(std::vector<Argument> &arguments);
};

#endif  // DEMOAPP_UTILS_H