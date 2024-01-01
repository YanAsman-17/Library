#include "MenuForm.h"
#include "StyleHelper.h"

MenuForm::MenuForm(int userId_, QMainWindow* parent) noexcept
    : QMainWindow(parent) , ui(new Ui::MenuForm), userId(userId_)
{
    ui->setupUi(this);
    setInterfaceStyle();
}

void MenuForm::setInterfaceStyle() noexcept {
    ui->centralwidget->setStyleSheet(StyleHelper::getCentralWidgetStyle());
    ui->libraryBtn->setStyleSheet(StyleHelper::getButtonsStyle());
    ui->logOutBtn->setStyleSheet(StyleHelper::getButtonsStyle());
    ui->userBooksBtn->setStyleSheet(StyleHelper::getButtonsStyle());
}

MenuForm::~MenuForm() noexcept
{
    delete ui;
}