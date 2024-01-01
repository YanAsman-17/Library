#pragma once
#include <qmainwindow.h>
#include "BookWidget.h"
#include "BooksOutput.h"

class UserBooksForm : public BooksOutput
{
	Q_OBJECT
public:
	explicit UserBooksForm(std::shared_ptr<sql::Connection> conn, int userId = 0, QWidget* parent = nullptr) noexcept;
	friend class Navigation;
private:
	bool historyMode;
	std::vector<std::unique_ptr<BookWidget>> books;

	void resetForm() noexcept;
	void booksOutput() noexcept;
	void resizeEvent(QResizeEvent* event) noexcept;
	void setInterfaceStyle() noexcept;
	std::string getStrUserBooks(std::string searchWords = "", bool audioBook = false, bool printBook = false, bool eBook = false,bool history = false) const noexcept;
};

