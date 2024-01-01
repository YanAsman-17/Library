#pragma once
#include "qmainwindow.h"
#include <mysql_connection.h>
#include "x64/Release/uic/ui_RegistrForm.h"

enum class lang {
	ERR,
	ENG,
	RUS
};

class RegistrForm : public QMainWindow
{
	Q_OBJECT
public:
	explicit RegistrForm(std::shared_ptr<sql::Connection> conn,QMainWindow* parent = nullptr) noexcept;
	~RegistrForm() noexcept;

	friend class Navigation;
private:

	Ui::RegistrForm* ui;
	std::shared_ptr<sql::Connection> conn;
	std::vector<QString> InpLinesErrors;
	bool isFirNameInpLineChange ,isLstNameInpLineChange ,isLogInpLineChange ,isPassInpLineChange;

	const int indexfrtNameLine;
	const int indexlstNameLine;
	const int indexLogLine;
	const int indexPassLine;

	void setInterfaceStyle() noexcept;
	bool eventFilter(QObject* object, QEvent* event) noexcept;
	
	void checkName(QLineEdit* qline) noexcept;
	void checkLogPass(QLineEdit* qline) noexcept;
	void checkLoginInDB(bool isLoginCorrect) noexcept;
	void resetForm() noexcept;
	bool outputError() noexcept;
	int addUser() noexcept;

};
