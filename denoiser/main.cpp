#include "denoisermain.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DenoiserMain w;
    w.show();
    return a.exec();
}
