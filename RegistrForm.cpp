#include <cppconn/resultset.h> 
#include <cppconn/prepared_statement.h>
#include <QRegExp>
#include "RegistrForm.h"
#include "StyleHelper.h"
#include "AuxilFunctns.h"
#include "sha256.h"

RegistrForm::RegistrForm(std::shared_ptr<sql::Connection> conn_,QMainWindow* parent) noexcept : QMainWindow(parent), ui(new Ui::RegistrForm) , 
conn(conn_),InpLinesErrors(4), isFirNameInpLineChange(false), isLstNameInpLineChange(false),
isLogInpLineChange(false), isPassInpLineChange(false) ,indexfrtNameLine(0),indexlstNameLine(1),indexLogLine(2),indexPassLine(3)
{
    ui->setupUi(this);
    setInterfaceStyle();

    ui->frtNameInpLine->installEventFilter(this);
    ui->lstNameInpLine->installEventFilter(this);
    ui->loginInpLine->installEventFilter(this);
    ui->passInpLine->installEventFilter(this);
    
    connect(ui->frtNameInpLine, &QLineEdit::textChanged, this, [this]() {
        checkName(ui->frtNameInpLine);
        });
    connect(ui->lstNameInpLine, &QLineEdit::textChanged, this, [this]() {
        checkName(ui->lstNameInpLine);
        });
    connect(ui->passInpLine, &QLineEdit::textChanged, this, [this]() {
        checkLogPass(ui->passInpLine);
        });
    connect(ui->loginInpLine, &QLineEdit::textChanged, this, [this]() {
        checkLogPass(ui->loginInpLine);
        });

    //resetForm() //если запускать RegistrForm по отдельности надо раскомментировать эту строку
}

void RegistrForm::resetForm() noexcept
{
    ui->title->setFocus();
    ui->frtNameInpLine->setText("Ян");
    ui->lstNameInpLine->setText("Асман");
    ui->loginInpLine->setText("janasman17");
    ui->passInpLine->setText("inkazilios_4195");
    ui->hint->setText("");
    std::fill(InpLinesErrors.begin(), InpLinesErrors.end(), QString());

    ui->frtNameInpLine->setStyleSheet(StyleHelper::getInputLineStyle());
    ui->lstNameInpLine->setStyleSheet(StyleHelper::getInputLineStyle());
    ui->loginInpLine->setStyleSheet(StyleHelper::getInputLineStyle());
    ui->passInpLine->setStyleSheet(StyleHelper::getInputLineStyle());

    isFirNameInpLineChange = false;
    isLstNameInpLineChange = false;
    isLogInpLineChange = false;
    isPassInpLineChange = false;
}

void RegistrForm::checkLoginInDB(bool isLoginCorrect) noexcept {
    try {
        std::string login = ui->loginInpLine->text().toStdString();
        std::string strCheckLogInBase = "SELECT id FROM users WHERE login LIKE BINARY ?";
        std::unique_ptr<sql::PreparedStatement> pstmtCheckLogInBase(conn->prepareStatement(strCheckLogInBase));
        pstmtCheckLogInBase->setString(1, login);
        std::unique_ptr<sql::ResultSet> resCheckLogInBase(pstmtCheckLogInBase->executeQuery());
        if (resCheckLogInBase->next() && isLoginCorrect) {
            InpLinesErrors[indexLogLine] = QString("К сожалению данный логин занят");
        }
        else if (isLoginCorrect) {
            InpLinesErrors[indexLogLine] = QString();
        }
        outputError();
    }
    catch (const sql::SQLException& ex) {
        ui->hint->setText("Произошла ошибка , попробуйте зайти позднее");
        std::cerr << "problem with connection in RegistrForm::checkLoginInDB" << std::endl;
    }
}

int RegistrForm::addUser() noexcept
{
    try {
        if (!outputError()) {
            std::string strAddUser = "INSERT INTO users (first_name,last_name,login,passwd,hashPass,salt) VALUES(?,?,?,?,?,?)";
            std::unique_ptr<sql::PreparedStatement> pstmtAddUser(conn->prepareStatement(strAddUser));

            std::string firstName = ui->frtNameInpLine->text().toStdString();
            std::string lastName = ui->lstNameInpLine->text().toStdString();
            std::string login = ui->loginInpLine->text().toStdString();
            std::string password = ui->passInpLine->text().toStdString();

            std::string sharSalt = getSharedSalt();
            std::string persSalt = getSalt();
            std::string intermHash = sha256(password + persSalt);
            std::string hashPass = sha256(intermHash + sharSalt);

            pstmtAddUser->setString(1, firstName);
            pstmtAddUser->setString(2, lastName);
            pstmtAddUser->setString(3, login);
            pstmtAddUser->setString(4, password);
            pstmtAddUser->setString(5, hashPass);
            pstmtAddUser->setString(6, persSalt);
            pstmtAddUser->execute();

            std::string strGetUserId = "SELECT id FROM users WHERE login LIKE BINARY ?"; 
            std::unique_ptr<sql::PreparedStatement> pstmtGetUserId(conn->prepareStatement(strGetUserId));
            pstmtGetUserId->setString(1,login);
            std::unique_ptr<sql::ResultSet> resGetUserId(pstmtGetUserId->executeQuery());
            if (resGetUserId->next()) {
                int userId = resGetUserId->getInt("id");
                return userId;
            }
        }
    }
    catch (const sql::SQLException& ex) {
        ui->hint->setText("Произошла ошибка , попробуйте зайти позднее");
        std::cerr << "problem with connection in RegistrForm::addUser" << std::endl;
    }
    return 0;
}

void RegistrForm::checkLogPass(QLineEdit* qline) noexcept
{
    QRegExp rx("([a-zA-Z]{1})"
        "([^а-яА-ЯёЁ]{5,29})"
    );
    const int indexLine = (qline == ui->loginInpLine) ? indexLogLine : indexPassLine;
    bool isLineCorrect = true;
    if (!rx.exactMatch(qline->text())) {
        InpLinesErrors[indexLine] = QString("Логин/пароль должны содержать от 6 до 30 символов(латиница,цифры,символы) \n первый символ - латиница");
        isLineCorrect = false;
    }
    else {
        InpLinesErrors[indexLine] = QString();
    }

    if (qline == ui->loginInpLine) {
        checkLoginInDB(isLineCorrect);
    }
    else {
        outputError();
    }

}

void RegistrForm::checkName(QLineEdit* qline) noexcept
{
    QString name = qline->text(); // под name может быть как имя так и фамилия
    const int indexLine = (qline == ui->frtNameInpLine) ? indexfrtNameLine : indexlstNameLine;

    QRegExp rx("([а-яА-ЯёЁ]*)");

    if (!rx.exactMatch(name)) {
        InpLinesErrors[indexLine] = QString("Имя/фамилия должны состоять только из кириллицы");
    }
    else if (name.length() < 2) {
        InpLinesErrors[indexLine] = QString("Имя/фамилия должны состоять как минимум из двух букв");
    }
    else if (name.length() > 30) {
        InpLinesErrors[indexLine] = QString("Имя/фамилия не должны превышать тридцати букв");
    }
    else {
        InpLinesErrors[indexLine] = QString();
    }
    name = name.toLower();
    if (name.length() > 0) {
        name[0] = name[0].toUpper();
        qline->setText(name);
    }
    
    outputError();
}

bool RegistrForm::outputError() noexcept {
    bool haveError = false;
    for (auto i = InpLinesErrors.begin(); i < InpLinesErrors.end(); ++i) {
        if (*i != QString()) {
            ui->hint->setText(*i);
            haveError = true;
        }
    }
    if (haveError == false) {
        ui->hint->setText("");
    }
    return haveError;
}

bool RegistrForm::eventFilter(QObject* object, QEvent* event) noexcept
{
    if (object == ui->frtNameInpLine && event->type() == QEvent::FocusIn && !isFirNameInpLineChange) { // Событие первого нажатия на строку ввода
        ui->frtNameInpLine->setText("");
        ui->frtNameInpLine->setStyleSheet(StyleHelper::getChangedInputLineStyle());
        isFirNameInpLineChange = true;
    }
    else if (object == ui->lstNameInpLine && event->type() == QEvent::FocusIn && !isLstNameInpLineChange) {
        ui->lstNameInpLine->setText("");
        ui->lstNameInpLine->setStyleSheet(StyleHelper::getChangedInputLineStyle());
        isLstNameInpLineChange = true;
    }
    else if (object == ui->loginInpLine && event->type() == QEvent::FocusIn && !isLogInpLineChange) {
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

void RegistrForm::setInterfaceStyle() noexcept
{
    ui->centralwidget->setStyleSheet(StyleHelper::getCentralWidgetStyle());
    ui->mainWidget->setStyleSheet(StyleHelper::getMainWidgetStyle());
    ui->regBtn->setStyleSheet(StyleHelper::getButtonsStyle());
    ui->frtNameInpLine->setStyleSheet(StyleHelper::getInputLineStyle());
    ui->lstNameInpLine->setStyleSheet(StyleHelper::getInputLineStyle());
    ui->loginInpLine->setStyleSheet(StyleHelper::getInputLineStyle());
    ui->passInpLine->setStyleSheet(StyleHelper::getInputLineStyle());
    ui->lblHaveAccount->setStyleSheet(StyleHelper::getHaveAccountLblStyle());
}

RegistrForm::~RegistrForm() noexcept {
    delete ui;
}