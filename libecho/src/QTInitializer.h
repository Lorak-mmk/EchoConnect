#include <QtCore/QThread>
#include <QtCore/QCoreApplication>

#include "echo.h"

class QTInitializer : public QObject {
    Q_OBJECT
public:
    QTInitializer(int argc, char* argv[]);
    ~QTInitializer();

private:
    static QCoreApplication *app;
};