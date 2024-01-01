#include <cppconn/resultset.h> 
#include <cppconn/prepared_statement.h>
#include <QRegExp>
#include "qlineedit.h"
#include "StyleHelper.h"
#include "sha256.h"
#include "LoginForm.h"
#include "AuxilFunctns.h"



LoginForm::LoginForm(std::shared_ptr<sql::Connection> conn_,QMainWindow* parent) noexcept
    : QMainWindow(parent) , conn(conn_)
    , ui(new Ui::LoginForm) , isLogInpLineChange(false), isPassInpLineChange(false), lenTextPrev(0)
{
    ui->setupUi(this);

    setInterfaceStyle();
    ui->loginInpLine->installEventFilter(this);
    ui->passInpLine->installEventFilter(this);

    //resetForm(); //если запускать LoginForm по отдельности надо раскомментировать эту строку

}

void LoginForm::resetForm() noexcept
{
    ui->title->setFocus();
    ui->loginInpLine->setText("janasman17");
    ui->passInpLine->setText("Q34fs_pfeas");
    ui->hint->setText("");
    lenTextPrev = 0;

    ui->loginInpLine->setStyleSheet(StyleHelper::getInputLineStyle());
    ui->passInpLine->setStyleSheet(StyleHelper::getInputLineStyle());

    isLogInpLineChange = false;
    isPassInpLineChange = false;
}

UserInfrom LoginForm::checkUser() noexcept {  //проверяем есть ли пользователь в бд,если есть возвращем его id,ялвяется ли админом

    std::string login = ui->loginInpLine->text().toStdString();
    std::string pass = ui->passInpLine->text().toStdString();

    try {
        std::string strGetUserSalt = "SELECT salt FROM users WHERE login LIKE BINARY ?";
        std::unique_ptr<sql::PreparedStatement> pstmtGetUserSalt(conn->prepareStatement(strGetUserSalt));
        pstmtGetUserSalt->setString(1, login);
        std::unique_ptr<sql::ResultSet> resGetUserSalt(pstmtGetUserSalt->executeQuery());
        if (resGetUserSalt->next()) {
            std::string salt = resGetUserSalt->getString(1);
            std::string intermHash = sha256(pass + salt);
            std::string sharSalt = getSharedSalt();
            std::string hashPass = sha256(intermHash + sharSalt);

            std::string strCheckUserInBd = "SELECT id ,isAdmin FROM users WHERE login LIKE BINARY ? AND hashPass LIKE BINARY ?";
            std::unique_ptr<sql::PreparedStatement> pstmtCheckUserInBd(conn->prepareStatement(strCheckUserInBd));
            pstmtCheckUserInBd->setString(1, login);
            pstmtCheckUserInBd->setString(2, hashPass);
            std::unique_ptr<sql::ResultSet> resCheckUserInBd(pstmtCheckUserInBd->executeQuery());

            if (resCheckUserInBd->next() != 0) {
                int userId =  resCheckUserInBd->getInt("id");
                bool isAdmin = resCheckUserInBd->getInt("isAdmin");
                return UserInfrom(userId,isAdmin);
            }
        }
        ui->hint->setText("Неверный логин или пороль");
    }
    catch (const sql::SQLException& ex) {
        ui->hint->setText("Произошла ошибка при входе ,повторите попытку или вернитесь позднее");
        std::cerr << "problem with connection in LoginForm::checkUser" << std::endl;

    }
    return UserInfrom(0, false);
}

bool LoginForm::eventFilter(QObject* object, QEvent* event) noexcept
{
    if (object == ui->loginInpLine && event->type() == QEvent::FocusIn && !isLogInpLineChange) { // Событие первого нажатия на строку ввода
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


void LoginForm::setInterfaceStyle() noexcept
{
    ui->centralwidget->setStyleSheet(StyleHelper::getCentralWidgetStyle());
    ui->mainWidget->setStyleSheet(StyleHelper::getMainWidgetStyle());
    ui->entryBtn->setStyleSheet(StyleHelper::getButtonsStyle());
    ui->regBtn->setStyleSheet(StyleHelper::getButtonsStyle());
    ui->loginInpLine->setStyleSheet(StyleHelper::getInputLineStyle());
    ui->passInpLine->setStyleSheet(StyleHelper::getInputLineStyle());
}

LoginForm::~LoginForm() noexcept
{
    delete ui;
}

UserInfrom::UserInfrom(int userId, bool isAdmin)
{
    this->userId = userId;
    this->isAdmin = isAdmin;
}
