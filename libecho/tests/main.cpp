#include <echo.h>
#include <gtest/gtest.h>

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    echo::initQT(argc, argv);
    return RUN_ALL_TESTS();
}