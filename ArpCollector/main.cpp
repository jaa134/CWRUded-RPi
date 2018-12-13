#include "service.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Service *s = new Service("http://cwruded.herokuapp.com/api/updateLocation", "Home");
    s->start();

    return a.exec();
}
