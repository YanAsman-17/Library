
#pragma once
#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include "qevent.h"
#include "LoginForm.h"
#include "RegistrForm.h"
#include "MenuForm.h"
#include "LibBooksForm.h"
#include "CheckBookForm.h"
#include "UserBooksForm.h"
#include "BookReaderForm.h"
#include "AdminMenuForm.h"
#include "AdminMainForm.h"
#include "NavigationIndexes.h"

class Navigation : public QMainWindow
{
    Q_OBJECT
public:
    explicit Navigation(QWidget* parent = nullptr) noexcept;
private:
    std::unique_ptr<QStackedWidget> stackedWidget;

    std::unique_ptr<LoginForm> logIn;
    std::unique_ptr<RegistrForm> registr;
    std::unique_ptr<MenuForm> menu;
    std::unique_ptr<LibBooksForm> libBooks;
    std::unique_ptr<CheckBookForm> checkBook;
    std::unique_ptr<UserBooksForm> userBooks;
    std::unique_ptr<BookReaderForm> bookReader;
    std::unique_ptr<AdminMenuForm> adminMenu;
    std::unique_ptr<AdminMainForm> adminMain;

    std::shared_ptr<sql::Connection> conn;

    bool eventFilter(QObject* object, QEvent* event) noexcept;
};
