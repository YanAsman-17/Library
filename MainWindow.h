#include <QMainWindow>
#include "ui_mainwindow.h"
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h> 
#include <cppconn/statement.h>
#include "User.h"
#include <cppconn/prepared_statement.h>
#include <QKeyEvent>
#include <qapplication.h>
#include <string>
#include <sstream>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    bool eventFilter(QObject* object, QEvent* event);
    void hidePass1();
private slots:
    User checkUser();

private:
    Ui::MainWindow* ui;
    void setInterfaceStyle();
    std::unique_ptr<sql::Connection> conn;
    std::stringstream password;
};
