#include <gtest/gtest.h>

#include "Echo.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    Echo::initEcho(argc, argv);
    return RUN_ALL_TESTS();
}
