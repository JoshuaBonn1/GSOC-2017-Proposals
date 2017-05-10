#include "timeline.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Timeline w;
    w.show();

    return a.exec();
}
