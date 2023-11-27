#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "qlineedit.h"
#include "StyleHelper.h"
#include "User.h"
#include "sha256.h"
#include "string"
#include <iostream>
#include "AuxilFunctns.h"

using namespace std;


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) , conn(nullptr)
{
    ui->setupUi(this);
    setInterfaceStyle();
    ui->loginInpLine->installEventFilter(this);
    ui->passInpLine->installEventFilter(this);

    try {
        sql::Driver* driver = get_driver_instance();
        conn.reset(driver->connect("tcp://127.0.0.1:3306", "root", "kriogen701"));
        conn->setSchema("diglib");
    }
    catch (...) {
        cerr << "Error" << endl;
    }
    connect(ui->entryBtn, &QPushButton::clicked, this, &MainWindow::checkUser);
    connect(ui->passInpLine, &QLineEdit::textEdited, this, &MainWindow::hidePass1);
}

void MainWindow::hidePass1() {
    int textLen = ui->passInpLine->text().length();
    QChar addSymb = ui->passInpLine->text()[textLen - 1];
    password << QString(addSymb).toUtf8().constData();
    string stars(textLen, '*');
    ui->passInpLine->setText(QString::fromUtf8(stars));
}

bool MainWindow::eventFilter(QObject* object, QEvent* event)
{
    static bool isLogInpLineChange = false;
    static bool isPassInpLineChange = false;

    if (object == ui->loginInpLine && event->type() == QEvent::FocusIn && !isLogInpLineChange) {
        ui->loginInpLine->setText("");
        ui->loginInpLine->setStyleSheet(StyleHelper::getChangedInputLineStyle());
        isLogInpLineChange = true;
    }
    else if (object == ui->passInpLine && event->type() == QEvent::FocusIn && !isPassInpLineChange) {
        ui->passInpLine->setText("");
        ui->passInpLine->setStyleSheet(StyleHelper::getChangedInputLineStyle());
        isPassInpLineChange = true;
    }
    return false;
}


User MainWindow::checkUser() {

    string login = ui->loginInpLine->text().toStdString();
    string pass = password.str();

    cout << pass << endl;
    try {
        string strGetUserSalt = "SELECT salt FROM users WHERE login LIKE BINARY ?";
        unique_ptr<sql::PreparedStatement> pstmtGetUserSalt(conn->prepareStatement(strGetUserSalt));
        pstmtGetUserSalt->setString(1, login);
        unique_ptr<sql::ResultSet> resGetUserSalt(pstmtGetUserSalt->executeQuery());
        if (resGetUserSalt->next()) {
            string salt = resGetUserSalt->getString(1);
            string intermHash = sha256(pass + salt);
            string sharSalt = getSharedSalt();
            string hashPass = sha256(intermHash + sharSalt);

            string strCheckUserInBd = "SELECT id FROM users WHERE login LIKE BINARY ? AND hashPass LIKE BINARY ?";
            unique_ptr<sql::PreparedStatement> pstmtCheckUserInBd(conn->prepareStatement(strCheckUserInBd));
            pstmtCheckUserInBd->setString(1, login);
            pstmtCheckUserInBd->setString(2, hashPass);
            unique_ptr<sql::ResultSet> resCheckUserInBd(pstmtCheckUserInBd->executeQuery());

            if (resCheckUserInBd->next() != 0) {
                ui->hint->setText("Вы вошли в личный кабинет");
                User user(resCheckUserInBd->getInt("id"));
                return user;
            }
        }
        ui->hint->setText("Неверный логин или пороль");
    }
    catch (const sql::SQLException& ex) {
        ui->hint->setText("Произошла ошибка при вхоже ,повторите попытку или вернитесь позднее");
        return User(-1);
    }
    return User(-1);
}


MainWindow::~MainWindow()
{
    delete ui;
    //conn->close();
}


void MainWindow::setInterfaceStyle()
{
    ui->centralwidget->setStyleSheet(StyleHelper::getWidgetStyle());
    ui->entryBtn->setStyleSheet(StyleHelper::getButtonsStyle());
    ui->regBtn->setStyleSheet(StyleHelper::getButtonsStyle());
    ui->loginInpLine->setStyleSheet(StyleHelper::getInputLineStyle());
    ui->passInpLine->setStyleSheet(StyleHelper::getInputLineStyle());
}

