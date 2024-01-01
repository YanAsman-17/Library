#pragma once
#include <QMainWindow>
#include <QKeyEvent>
#include <qapplication.h>
#include <mysql_connection.h>
#include "x64/Release/uic/ui_LoginForm.h"

struct UserInfrom {
    UserInfrom(int userId, bool isAdmin);
    int userId;
    bool isAdmin;
};

class LoginForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginForm(std::shared_ptr<sql::Connection> conn , QMainWindow* parent = nullptr) noexcept;
    ~LoginForm() noexcept;
    friend class Navigation;

signals:
    void checkBook();
private slots:
    UserInfrom checkUser() noexcept;
private:
    Ui::LoginForm* ui;
    std::shared_ptr<sql::Connection> conn;
    bool isLogInpLineChange, isPassInpLineChange;
    int lenTextPrev;

    void setInterfaceStyle() noexcept;
    bool eventFilter(QObject* object, QEvent* event) noexcept;
    void resetForm() noexcept;
};