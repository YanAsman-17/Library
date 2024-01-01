#include <qapplication.h>
#include "Navigation.h"
#include "LoginForm.h"
#include "RegistrForm.h"
#include <Windows.h>
#include "BookWidget.h"
//#include "vld.h"

int main(int argc, char *argv[])
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    QApplication a(argc, argv);
    Navigation window;
    window.show(); 
    
    return a.exec();

}
