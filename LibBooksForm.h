#pragma once
#include <qmainwindow.h>
#include <mysql_connection.h>
#include "BooksOutput.h"

class BookWidget;

class LibBooksForm : public BooksOutput
{
	Q_OBJECT
public:
	explicit LibBooksForm(std::shared_ptr<sql::Connection> conn,int userId = 0,QWidget* parent = nullptr) noexcept;
	~LibBooksForm() noexcept;
	friend class Navigation;
private:
	std::vector<std::unique_ptr<BookWidget>> allBooks;
	std::vector<std::unique_ptr<BookWidget>> searchBooks;

	void resetForm() noexcept;
	void booksOutput() noexcept;
	void resizeEvent(QResizeEvent* event) noexcept;
	std::string getStrLibBooks() const;
	std::string getStrSearchLibBooks(std::string searchWords = "", bool audioBook = false, bool printBook = false, bool eBook = false) const ;
};
