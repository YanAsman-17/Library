#pragma once
#include "qmainwindow.h"
#include "x64/Release/uic/ui_CheckBookForm.h"
#include <mysql_connection.h>
#include "GetPrintBookForm.h"
#include "NavigationIndexes.h"

enum class BookFormatStatus {
	USER_RESERVED,
	USER_HAVE,
	NOT_IN_THE_LIBRARY,
	ENDED,
	IN_STOCK
};
enum class BookFormat {
	E_BOOK,
	AUDIOBOOK,
	PRINT_BOOK
};
struct TypeBookInform {
	BookFormatStatus status;
	QString type;
	QLabel* bookInform;
	QPushButton* bookBtn;
	QPushButton* bookRetBtn;
	QLabel* hint;
	TypeBookInform(BookFormatStatus status,QString type, QLabel* bookInform, QPushButton* bookBtn,QPushButton* bookRetBtn, QLabel* hint);
};

class CheckBookForm : public QMainWindow
{
	Q_OBJECT
public:
	explicit CheckBookForm(std::shared_ptr<sql::Connection> conn,NavigationIndexes prevForm_ = NavigationIndexes::LIB_BOOKS, 
		int userId_ = 0, int bookId_ = 0, QWidget* parent = nullptr) noexcept;
	~CheckBookForm() noexcept;
	friend class Navigation;
signals:
	void openEBookSignal(QString bookPath, QString bookName);
	void confirmTheReservationSignal();
public slots:
	void printBtnClickHandler() noexcept;
private:
	Ui::CheckBookForm* ui;
	std::shared_ptr<sql::Connection> conn;
	int userId,bookId;
	BookFormatStatus printBook, eBook, audioBook;
	NavigationIndexes prevForm;
	GetPrintBookForm* printBookForm;
	
	void setInterfaceStyle() noexcept;
	void getBookInformationFromDB();
	void resetForm(NavigationIndexes prevForm, int userId, int bookId) noexcept;

	bool giveBookToUser(BookFormat format) noexcept;
	bool reservPrintBook() noexcept;
	void openEBook() noexcept;
	bool returnBook(BookFormat format) ;
	bool checkErrors(int bookTypeId, QLabel* hint) noexcept;
	std::pair<std::string,std::string> getDeadlineReservAndCode(int bookTypeId) const ;
	std::pair<std::string, std::string> getDeadlineTakenAndCode(int bookTypeId) const ;

	int getBookTypeId(std::string bookType) const;
	bool getHaveUserDebt() const ;
	int getCntUserBooks() const ;
	std::string getCodeForBookOrder() const ;
	bool getCheckBookAvailab(int bookTypeId) const;
	std::pair<bool,int> getCheckBookUserRecently(int bookTypeId) const;
	
	void resizeEvent(QResizeEvent* event) noexcept;

};

