#include <QtCore/QCoreApplication>

#include "App.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    new App();

    return a.exec();
}
