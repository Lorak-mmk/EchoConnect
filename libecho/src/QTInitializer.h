#ifndef ECHOCONNECT_QTINITIALIZER_H
#define ECHOCONNECT_QTINITIALIZER_H

#include <QtCore/QCoreApplication>

/**
 * TODO
 */
class QTInitializer {
public:
    /**
     * @brief If QApplication isn't already initialized, creates QCoreApplication.
     *
     * @param a_argc    Program's argc from main().
     * @param a_argv    Program's argv from main().
     */
    QTInitializer(int a_argc, char **a_argv);
    ~QTInitializer();

private:
    QCoreApplication *app;
};

#endif  // ECHOCONNECT_QTINITIALIZER_H
