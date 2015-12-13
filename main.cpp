#include "elistener.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EListener w;
    w.show();

    return a.exec();
}
