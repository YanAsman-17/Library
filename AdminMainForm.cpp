#include <cppconn/resultset.h> 
#include <cppconn/prepared_statement.h>
#include "AdminMainForm.h"
#include "StyleHelper.h"

AdminMainForm::AdminMainForm(std::shared_ptr<sql::Connection> conn_,QMainWindow* parent) noexcept : QMainWindow(parent) , ui(new Ui::AdminMainFrom) ,
conn(conn_),isSearchLineChange(false), bookTypeId(0) , userId(0) , status(BookStatus::NOT_FOUND)
{
	ui->setupUi(this);
	setInterfaceStyle();

	ui->searchLine->installEventFilter(this);
	
	connect(ui->searchLine, &QLineEdit::textChanged, this, [this]() {
		ui->errorMessage->setText("");
		if (ui->searchLine->text().length() == 6) { // 6 - длина кода пользователя
			requestHandler();
		}
		else {
			ui->bookName->hide();
			ui->statusWidget->hide();
			ui->userName->hide();
			ui->retIssueBtn->hide();
		}
		});

	connect(ui->retIssueBtn, &QPushButton::clicked, this, &AdminMainForm::changeBookStatus);

	//resetForm();

}
void AdminMainForm::resetForm() noexcept
{
	ui->inputCodeText->setFocus();
	ui->searchLine->setText("458343");
	ui->searchLine->setStyleSheet(StyleHelper::getInputLineStyle());
	isSearchLineChange = false;

	status = BookStatus::NOT_FOUND;
	bookTypeId = 0;
	userId = 0;

	isSearchLineChange = false;
	ui->errorMessage->setText("");

	ui->bookName->hide();
	ui->statusWidget->hide();
	ui->userName->hide();
	ui->retIssueBtn->hide();

}

void AdminMainForm::requestHandler() noexcept { // выводит информацию о статусе книги
	try {
		requestInform();
		ui->bookName->show();
		ui->statusWidget->show();
		ui->retIssueBtn->show();
		std::string bookName = getBookName();
		ui->bookName->setText(QString::fromUtf8(bookName));
		if (status == BookStatus::TAKEN) {
			ui->statusBook->setText("у клиента");
			ui->retIssueBtn->setText("Книга принята");
		}
		else if (status == BookStatus::RESERVED) {
			ui->statusBook->setText("забронирована");
			ui->retIssueBtn->setText("Книга выдана");
		}
		else if (status == BookStatus::RETURNED) {
			ui->statusBook->setText("возвращена в библиотеку");
			ui->retIssueBtn->hide();
		}
		else if (status == BookStatus::NOT_FOUND) {
			ui->statusBook->setText("Не найдена");
			ui->bookName->hide();
			ui->userName->hide();
			ui->retIssueBtn->hide();
		}
	} 
	catch (const sql::SQLException& ex) {
		std::cerr << "problem with connection in AdminMainForm::requestHandler" << std::endl;
		ui->errorMessage->setText("Произошла ошибка,вернитесь позднее");
	}
}

void AdminMainForm::requestInform()
{
	status = BookStatus::NOT_FOUND;
	bookTypeId = 0;
	userId = 0;

	//книга которую зарезервировали 
	std::string strBookTypeIdReservBook = "SELECT book_type_id,user_id FROM users_books WHERE date_reserv IS NOT NULL AND date_taken IS NULL AND code = ?";
	std::unique_ptr<sql::PreparedStatement> pstmtBookTypeIdReservBook(conn->prepareStatement(strBookTypeIdReservBook));
	std::string code = ui->searchLine->text().toUtf8().constData();
	pstmtBookTypeIdReservBook->setString(1, code);
	std::unique_ptr<sql::ResultSet> resBookTypeIdReservBook(pstmtBookTypeIdReservBook->executeQuery());
	if (resBookTypeIdReservBook->next()) {
		bookTypeId = resBookTypeIdReservBook->getInt("book_type_id");
		userId = resBookTypeIdReservBook->getInt("user_id");
		status = BookStatus::RESERVED;
		return;
	}

	//книга которую взяли
	std::string strBookTypeIdTakenBook = "SELECT book_type_id,user_id FROM users_books WHERE date_taken IS NOT NULL AND date_return IS NULL AND code = ?";
	std::unique_ptr<sql::PreparedStatement> pstmtBookTypeIdTakenBook(conn->prepareStatement(strBookTypeIdTakenBook));
	code = ui->searchLine->text().toUtf8().constData();
	pstmtBookTypeIdTakenBook->setString(1, code);
	std::unique_ptr<sql::ResultSet> resBookTypeIdTakenBook(pstmtBookTypeIdTakenBook->executeQuery());
	if (resBookTypeIdTakenBook->next()) {
		bookTypeId = resBookTypeIdTakenBook->getInt("book_type_id");
		userId = resBookTypeIdTakenBook->getInt("user_id");
		status = BookStatus::TAKEN;
		return;
	}

	//книга которую уже вернули 
	std::string strBookTypeIdReturnBook = "SELECT book_type_id,user_id FROM users_books WHERE date_return IS NOT NULL AND code = ?";
	std::unique_ptr<sql::PreparedStatement> pstmtBookTypeIdReturnBook(conn->prepareStatement(strBookTypeIdReturnBook));
	code = ui->searchLine->text().toUtf8().constData();
	pstmtBookTypeIdReturnBook->setString(1, code);
	std::unique_ptr<sql::ResultSet> resBookTypeIdReturnBook(pstmtBookTypeIdReturnBook->executeQuery());
	if (resBookTypeIdReturnBook->next()) {
		bookTypeId = resBookTypeIdReturnBook->getInt("book_type_id");
		userId = resBookTypeIdReturnBook->getInt("user_id");
		status = BookStatus::RETURNED;
		return;
	}
	
}

std::string AdminMainForm::getBookName() const 
{
	std::string name;
	std::string strBookTypeIdReturnBook = "SELECT name FROM books "
		"LEFT JOIN books_types ON books.id = books_types.book_id "
		"WHERE books_types.id = ? ";
	std::unique_ptr<sql::PreparedStatement> pstmtBookTypeIdReturnBook(conn->prepareStatement(strBookTypeIdReturnBook));
	pstmtBookTypeIdReturnBook->setInt(1, bookTypeId);
	std::unique_ptr<sql::ResultSet> resBookTypeIdReturnBook(pstmtBookTypeIdReturnBook->executeQuery());
	if (resBookTypeIdReturnBook->next()) {
		name = resBookTypeIdReturnBook->getString("name");
	}
	return name;
}

bool AdminMainForm::giveBookToUser() noexcept
{
	std::unique_ptr<sql::Statement> stmtIsolLevel(conn->createStatement());
	try {
		stmtIsolLevel->execute("SET SESSION TRANSACTION ISOLATION LEVEL SERIALIZABLE");
		conn->setAutoCommit(false);

		std::string strRemBookFromLib = "UPDATE books_types SET in_stock = in_stock - 1 WHERE id = ?";
		std::unique_ptr<sql::PreparedStatement> pstmtRemBookFromLib(conn->prepareStatement(strRemBookFromLib));
		pstmtRemBookFromLib->setInt(1, bookTypeId);
		pstmtRemBookFromLib->executeUpdate();

		std::string strAddBookToUser = "UPDATE users_books SET date_taken = CURDATE() WHERE user_id = ? AND book_type_id = ? AND date_taken IS NULL";
		std::unique_ptr<sql::PreparedStatement> pstmtAddBookToUser(conn->prepareStatement(strAddBookToUser));
		pstmtAddBookToUser->setInt(1, userId);
		pstmtAddBookToUser->setInt(2, bookTypeId);
		pstmtAddBookToUser->execute();

		conn->commit();
		conn->setAutoCommit(true);
		stmtIsolLevel->execute("SET SESSION TRANSACTION ISOLATION LEVEL READ COMMITTED");
		return true;
	}
	catch (const sql::SQLException& ex) {
		conn->rollback();
		conn->setAutoCommit(true);
		stmtIsolLevel->execute("SET SESSION TRANSACTION ISOLATION LEVEL READ COMMITTED");
		std::cerr << "problem with connection in CheckBookForm::giveBookToUser" << std::endl;
		return false;
	}
}


bool AdminMainForm::returnBook() noexcept // вернуть книгу в бибилотеку
{
	std::unique_ptr<sql::Statement> stmtIsolLevel(conn->createStatement());
	try {
		stmtIsolLevel->execute("SET SESSION TRANSACTION ISOLATION LEVEL SERIALIZABLE");
		conn->setAutoCommit(false);

		std::string strDelUserBook = "UPDATE users_books SET date_return = CURDATE() WHERE book_type_id = ? AND user_id = ? AND date_return IS NULL";
		std::unique_ptr<sql::PreparedStatement> pstmtDelUserBook(conn->prepareStatement(strDelUserBook));
		pstmtDelUserBook->setInt(1, bookTypeId);
		pstmtDelUserBook->setInt(2, userId);
		pstmtDelUserBook->execute();

		std::string strAddBookInLib = "UPDATE books_types SET in_stock = in_stock + 1 WHERE id = ?";
		std::unique_ptr<sql::PreparedStatement> pstmtAddBookInLib(conn->prepareStatement(strAddBookInLib));
		pstmtAddBookInLib->setInt(1, bookTypeId);
		pstmtAddBookInLib->execute();

		conn->commit();
		conn->setAutoCommit(true);
		stmtIsolLevel->execute("SET SESSION TRANSACTION ISOLATION LEVEL READ COMMITTED");
		return true;
	}
	catch (const sql::SQLException& ex) {
		conn->rollback();
		conn->setAutoCommit(true);
		stmtIsolLevel->execute("SET SESSION TRANSACTION ISOLATION LEVEL READ COMMITTED");
		std::cerr << "problem with connection in CheckBookForm::returnBook" << std::endl;
		return false;
	}

}


void AdminMainForm::changeBookStatus() noexcept // срабатывает при нажатии кнопки возврата/взятия книги
{
	if (status == BookStatus::TAKEN) {
		if (returnBook()) {
			status = BookStatus::RETURNED;
			requestHandler();
		}
		else {
			ui->errorMessage->setText("Произошла ошибка,книга не была возвращена");
		}
	}
	else if (status == BookStatus::RESERVED) {
		if (giveBookToUser()) {
			status = BookStatus::TAKEN;
			requestHandler();
		}
		else {
			ui->errorMessage->setText("Произошла ошибка,не передавайте книгу клиенту");
		}
	}
}

bool AdminMainForm::eventFilter(QObject* object, QEvent* event) noexcept
{
	if (object == ui->searchLine && event->type() == QEvent::FocusIn && !isSearchLineChange) {
		ui->searchLine->setText("");
		ui->searchLine->setStyleSheet(StyleHelper::getChangedInputLineStyle());
		isSearchLineChange = true;
	}
	return false;
}

void AdminMainForm::setInterfaceStyle() noexcept
{
	ui->centralwidget->setStyleSheet(StyleHelper::getCentralWidgetStyle());
	ui->mainWidget->setStyleSheet(StyleHelper::getMainWidgetStyle());
	ui->retIssueBtn->setStyleSheet(StyleHelper::getButtonsStyle());
	ui->searchLine->setStyleSheet(StyleHelper::getInputLineStyle());
	ui->backBtn->setStyleSheet(StyleHelper::getBackBtnStyle());
}

AdminMainForm::~AdminMainForm() noexcept
{
	delete ui;
}