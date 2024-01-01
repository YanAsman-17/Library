#include "GetPrintBookForm.h"
#include "StyleHelper.h"

GetPrintBookForm::GetPrintBookForm(QWidget* parent) noexcept : QMainWindow(parent),ui(new Ui::GetPrintBookForm)
{
	ui->setupUi(this);

	QIcon icon("resources/images/iconApp.png");
	setWindowIcon(icon);

	setInterfaceStyle();

	connect(ui->cancelBtn, &QPushButton::clicked, this, [=]() {
		this->close();
		});
}

void GetPrintBookForm::setInterfaceStyle() noexcept
{
	ui->centralwidget->setStyleSheet(StyleHelper::getCentralWidgetStyle());
	ui->cancelBtn->setStyleSheet(StyleHelper::getButtonsStyle());
	ui->confirmBtn->setStyleSheet(StyleHelper::getButtonsStyle());
}

GetPrintBookForm::~GetPrintBookForm() noexcept
{
	delete ui;
}

