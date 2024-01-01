#pragma once
#include <qmainwindow.h>
#include "x64/Release/uic/ui_BooksOutput.h"
#include <mysql_connection.h>
#include <cppconn/resultset.h> 

class BookWidget;

class BooksOutput : public QMainWindow
{
	Q_OBJECT
public:
	explicit BooksOutput(std::shared_ptr<sql::Connection> conn,int userId = 0, QWidget* parent = nullptr) noexcept;
	~BooksOutput() noexcept;
	int getUserId() const;
	friend class Navigation;
signals:
	void checkBook(int userId, int bookId); // вызывается из BookWidget::eventFilter
protected:
	Ui::BooksOutput* ui;
	std::shared_ptr<sql::Connection> conn;
	int userId;
	int cntColumn; // кол-во колонок в выводе книг 
	bool changeCntColumn;
	bool isSearchLineChange;
	QString prevSearchText;
	bool isSearch;
	bool isCheckBoxesActive;

	void resizeEvent(QResizeEvent* event) noexcept;
	std::unique_ptr<BookWidget> createBookWidget(std::unique_ptr<sql::ResultSet>& resGetCatalogBooks) noexcept;
	void resetForm() noexcept;
	bool eventFilter(QObject* object, QEvent* event) noexcept;
	void setInterfaceStyle() noexcept;
};