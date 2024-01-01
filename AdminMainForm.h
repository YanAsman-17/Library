#pragma once
#include <qmainwindow.h>
#include "x64/Release/uic/ui_AdminMainForm.h"
#include <mysql_connection.h>

enum class BookStatus {
	RESERVED,
	TAKEN,
	RETURNED,
	NOT_FOUND
};

class AdminMainForm :public QMainWindow
{
public:
	explicit AdminMainForm(std::shared_ptr<sql::Connection> conn,QMainWindow* parent = nullptr) noexcept;
	~AdminMainForm() noexcept;
	friend class Navigation;
public slots:
	void requestHandler() noexcept;
	void changeBookStatus() noexcept;
private:
	Ui::AdminMainFrom* ui;
	std::shared_ptr<sql::Connection> conn;
	BookStatus status;
	int bookTypeId;
	int userId;
	bool isSearchLineChange;

	bool eventFilter(QObject* object, QEvent* event) noexcept;
	void requestInform(); 
	std::string getBookName() const ;
	bool giveBookToUser() noexcept;
	bool returnBook() noexcept;
	void setInterfaceStyle() noexcept;
	void resetForm() noexcept;
};

