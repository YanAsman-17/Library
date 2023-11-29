#pragma once
#include <QMainWindow>

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h> 
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include "User.h"
#include <QKeyEvent>
#include <qapplication.h>
#include "x64/Release/uic/ui_LoginForm.h"
#include <string>
#include <sstream>


class LoginForm : public QMainWindow
{
    Q_OBJECT

public:
    friend class Navigation;
    LoginForm(QMainWindow* parent = nullptr);
    ~LoginForm();
    bool eventFilter(QObject* object, QEvent* event);
private slots:
    User checkUser();

private:
    void setInterfaceStyle();
    void hidePass();
    Ui::LoginForm* ui;
    std::unique_ptr<sql::Connection> conn;
    std::stringstream password;

};