#include <qwidget.h>
#include <qapplication.h>
#include "AuxilFunctns.h"
#include "MainWindow.h"
#include <iostream>
#include <Windows.h>
using namespace std;



int main(int argc, char *argv[])
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    QApplication a(argc, argv);
    MainWindow window;
    window.show();    
    return a.exec();
    
    return 0;
}
