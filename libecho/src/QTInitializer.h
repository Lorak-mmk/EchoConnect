#ifndef ECHOCONNECT_QTINITIALIZER_H
#define ECHOCONNECT_QTINITIALIZER_H

#include <QtCore/QCoreApplication>

class QTInitializer {
public:
    QTInitializer(int a_argc, char **a_argv);
    ~QTInitializer();

private:
    QCoreApplication *app;
};

#endif // ECHOCONNECT_QTINITIALIZER_H