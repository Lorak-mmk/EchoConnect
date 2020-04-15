#ifndef ECHOCONNECT_ECHO_H
#define ECHOCONNECT_ECHO_H

#include "EchoRawConnection.h"
#include "QTInitializer.h"

/**
 * @brief Public API for possibly neccessary libecho initialization. Also, includes other libecho API headers.
 *
 * As it includes every other libecho API classes it is possible to include only "Echo.h" while using libecho.
 */
class Echo {
public:
    /**
     * @brief Initializes library, required if program doesn't need QT.
     *
     * If program doesn't use QT (by using QT I mean creating QApplication),
     * it needs to call this method before using this library.
     * If program uses QT, calling this method is unnecessary.
     * 
     * @warning This method must never be called before creating QApplication manually!
     * 
     * @param a_argc    Program's argc from main().
     * @param a_argv    Program's argv from main().
     */
    static void initEcho(int a_argc, char **a_argv) {
        static QTInitializer init{a_argc, a_argv};
    }
};

#endif  // ECHOCONNECT_ECHO_H
