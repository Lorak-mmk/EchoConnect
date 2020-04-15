#ifndef ECHOCONNECT_ECHO_H
#define ECHOCONNECT_ECHO_H

#include "QTInitializer.h"

/**
 * @brief Public API for libecho.
 *
 * All functionalities provided by the library will be usable through Echo class methods.
 */
class Echo {
public:
    /**
     * @brief Initializes library, required if program doesn't need QT.
     *
     * If program doesn't use QT (by using QT I mean creating QApplication),
     * it needs to call this method before using this library.
     * If program uses QT, calling this method is unnecessary.
     * This method must never be called before creating QApplication manually!
     * @param a_argc    Program's argc from main().
     * @param a_argv    Program's argv from main().
     */
    static void initEcho(int a_argc, char **a_argv) {
        static QTInitializer init{a_argc, a_argv};
    }
};

#endif  // ECHOCONNECT_ECHO_H
