#include "service.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Service *s = new Service("Home");
    s->start();

    return a.exec();
}
