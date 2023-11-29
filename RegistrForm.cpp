#include "RegistrForm.h"
#include "StyleHelper.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h> 
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <string>

RegistrForm::RegistrForm(QMainWindow* parent) : QMainWindow(parent), ui(new Ui::RegistrForm)
{
    ui->setupUi(this);
    setInterfaceStyle();

    ui->tittle->setFocus();
    ui->frtNameInpLine->installEventFilter(this);
    ui->lstNameInpLine->installEventFilter(this);
    ui->loginInpLine->installEventFilter(this);
    ui->passInpLine->installEventFilter(this);

    connect(ui->passInpLine, &QLineEdit::textEdited, this, &RegistrForm::hidePass);

}

void RegistrForm::setInterfaceStyle()
{
    ui->centralwidget->setStyleSheet(StyleHelper::getCentralWidgetStyle());
    ui->widget->setStyleSheet(StyleHelper::getWidgetStyle());
    ui->regBtn->setStyleSheet(StyleHelper::getButtonsStyle());
    ui->frtNameInpLine->setStyleSheet(StyleHelper::getInputLineStyle());
    ui->lstNameInpLine->setStyleSheet(StyleHelper::getInputLineStyle());
    ui->loginInpLine->setStyleSheet(StyleHelper::getInputLineStyle());
    ui->passInpLine->setStyleSheet(StyleHelper::getInputLineStyle());
}

void RegistrForm::hidePass() {

    static int lenTextPrev = -1;
    int lenText = ui->passInpLine->text().length();

    if (lenText > lenTextPrev) {
        QChar addSymb = ui->passInpLine->text()[lenText - 1];
        password << QString(addSymb).toUtf8().constData();
    }
    else {
        std::string strPass = password.str();
        password.str(std::string());;
        password << strPass.substr(0, strPass.length() - 1);
    }
    std::string stars(lenText, '*');
    ui->passInpLine->setText(QString::fromUtf8(stars));

    lenTextPrev = lenText;
}


bool RegistrForm::eventFilter(QObject* object, QEvent* event)
{
    
    static bool isFirNameInpLineChange = false;
    static bool isLstNameInpLineChange = false;
    static bool isLogInpLineChange = false;
    static bool isPassInpLineChange = false;

    if (object == ui->frtNameInpLine && event->type() == QEvent::FocusIn && !isFirNameInpLineChange) {
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