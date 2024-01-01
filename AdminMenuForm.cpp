#include "AdminMenuForm.h"
#include "StyleHelper.h"

AdminMenuForm::AdminMenuForm(int userId_,QWidget* parent) noexcept : QMainWindow(parent),ui(new Ui::AdminMenuForm),userId(userId_)
{
	ui->setupUi(this);
	setInterfaceStyle();
}

void AdminMenuForm::setInterfaceStyle() noexcept
{
	ui->centralwidget->setStyleSheet(StyleHelper::getCentralWidgetStyle()); 
	ui->mainAdmBtn->setStyleSheet(StyleHelper::getButtonsStyle());
	ui->logOutBtn->setStyleSheet(StyleHelper::getButtonsStyle());
}

AdminMenuForm::~AdminMenuForm() noexcept
{
	delete ui;
}