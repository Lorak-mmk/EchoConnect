#include "QTInitializer.h"

#include <QtCore/QCoreApplication>

QTInitializer::QTInitializer(int a_argc, char **a_argv) {
    if (QCoreApplication::instance() == nullptr) {
        app = new QCoreApplication(a_argc, a_argv);
    }
}

QTInitializer::~QTInitializer() {
    delete app;
}

void echo::initEcho(int a_argc, char **a_argv) {
    static QTInitializer init(a_argc, a_argv);
}
