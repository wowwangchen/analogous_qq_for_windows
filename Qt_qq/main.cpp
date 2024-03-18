#include "Register.h"
#include"codeTranslate.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Register w;
    w.show();
    return app.exec();
}
