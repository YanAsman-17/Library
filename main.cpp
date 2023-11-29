#include <qapplication.h>
#include "AuxilFunctns.h"
#include "LoginForm.h"
#include "RegistrForm.h"
#include "Navigation.h"
#include <iostream>
#include <Windows.h>

using namespace std;



int main(int argc, char *argv[])
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    QApplication a(argc, argv);
    RegistrForm window;
    window.show();    
    return a.exec();
}
