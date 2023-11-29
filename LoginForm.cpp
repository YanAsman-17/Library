#include "LoginForm.h"
#include "qlineedit.h"
#include "StyleHelper.h"
#include "User.h"
#include "sha256.h"
#include "string"
#include <iostream>
#include "AuxilFunctns.h"

using namespace std;


LoginForm::LoginForm(QMainWindow* parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginForm) , conn(nullptr)
{
    ui->setupUi(this);
    setInterfaceStyle();
    ui->tittle->setFocus();
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
    connect(ui->entryBtn, &QPushButton::clicked, this, &LoginForm::checkUser);
    connect(ui->passInpLine, &QLineEdit::textEdited, this, &LoginForm::hidePass);
}

void LoginForm::hidePass() {

    static int lenTextPrev = -1;
    int lenText = ui->passInpLine->text().length();
  
    if (lenText > lenTextPrev) {
        QChar addSymb = ui->passInpLine->text()[lenText - 1];
        password << QString(addSymb).toUtf8().constData();
    }
    else {
        string strPass = password.str();
        password.str(std::string());;
        password << strPass.substr(0, strPass.length() - 1);
    }
    string stars(lenText, '*');
    ui->passInpLine->setText(QString::fromUtf8(stars));

    lenTextPrev = lenText;
}

bool LoginForm::eventFilter(QObject* object, QEvent* event)
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


User LoginForm::checkUser() {

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


LoginForm::~LoginForm()
{
    delete ui;
    conn->close();
}


void LoginForm::setInterfaceStyle()
{
    ui->centralwidget->setStyleSheet(StyleHelper::getCentralWidgetStyle());
    ui->widget->setStyleSheet(StyleHelper::getWidgetStyle());
    ui->entryBtn->setStyleSheet(StyleHelper::getButtonsStyle());
    ui->regBtn->setStyleSheet(StyleHelper::getButtonsStyle());
    ui->loginInpLine->setStyleSheet(StyleHelper::getInputLineStyle());
    ui->passInpLine->setStyleSheet(StyleHelper::getInputLineStyle());
}

