#include "echo.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QThread>

class QTInitializer : public QObject {
    Q_OBJECT
public:
    QTInitializer(int argc, char **argv);
    ~QTInitializer();

private:
    QCoreApplication *app;
};