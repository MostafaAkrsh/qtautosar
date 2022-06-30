#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include "inc/com.h"

extern Com_Type Com;
struct foo { int a; char c = 'a'; };

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Com_Init(&(Com.ComConfig));

        
    foo f = { .a = 42 };

    return a.exec();
}
