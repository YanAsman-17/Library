#pragma once
#include "sstream"
#include "qmainwindow.h"
#include <mysql_connection.h>
#include "x64/Release/uic/ui_RegistrForm.h"


class RegistrForm : public QMainWindow
{
	Q_OBJECT
public:
	RegistrForm(QMainWindow* parent = nullptr);
private:
	void setInterfaceStyle();
	void hidePass();
	bool eventFilter(QObject* object, QEvent* event);
	std::unique_ptr<sql::Connection> conn;
	std::stringstream password;
	Ui::RegistrForm* ui;
};

