#include "CustomGraphicsView.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CustomGraphicsView w;
    w.show();
    return a.exec();
}
