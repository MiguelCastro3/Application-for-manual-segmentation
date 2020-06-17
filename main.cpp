#include "aplicacao.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    aplicacao w;
    w.show();

    return a.exec();
}
