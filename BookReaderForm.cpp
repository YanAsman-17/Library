#include "BookReaderForm.h"
#include "qfile.h"
#include "qmessagebox.h"
#include "StyleHelper.h"

BookReaderForm::BookReaderForm(QMainWindow* parent) noexcept : QMainWindow(parent), ui(new Ui::BookReaderForm)
{
	ui->setupUi(this);
	//resetBook(bookPath,bookName); // расскоментируем эту строчку ,если запускаем это окно отдельно
}

void BookReaderForm::resetForm(QString bookPath, QString bookName) noexcept
{
	setWindowTitle(bookName);
	QFile file(bookPath);
	if (!file.open(QIODevice::ReadOnly)) {
		QMessageBox::information(nullptr, "info", file.errorString());
	}
	QTextStream in(&file);
	ui->book->setText(in.readAll());
	ui->centralwidget->setStyleSheet(StyleHelper::getCentralWidgetStyle());
	ui->backBtn->setStyleSheet(StyleHelper::getBackBtnStyle());
	ui->book->setStyleSheet("border:none");
}


BookReaderForm::~BookReaderForm() noexcept
{
	delete ui;
}
