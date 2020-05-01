#include "QTInitializer.h"

QTInitializer::QTInitializer(int a_argc, char **a_argv) {
    if (QCoreApplication::instance() == nullptr) {
        app = new QCoreApplication(a_argc, a_argv);
    }
    qSetMessagePattern("[%{type}] %{function} Thread %{threadid} - %{message}");
}

QTInitializer::~QTInitializer() {
    delete app;
}
