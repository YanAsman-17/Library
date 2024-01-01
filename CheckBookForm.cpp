#include <cppconn/resultset.h> 
#include <cppconn/prepared_statement.h>
#include "CheckBookForm.h"
#include "StyleHelper.h"
#include "sha256.h"
#include "AuxilFunctns.h"
#include "NavigationIndexes.h"

CheckBookForm::CheckBookForm(std::shared_ptr<sql::Connection> conn_,NavigationIndexes prevForm_,int userId_, int bookId_, QWidget* parent) noexcept:
	QMainWindow(parent), ui(new Ui::CheckBookForm),conn(conn_) , userId(userId_), bookId(bookId_),
	printBook(BookFormatStatus::NOT_IN_THE_LIBRARY),audioBook(BookFormatStatus::NOT_IN_THE_LIBRARY),
	eBook(BookFormatStatus::NOT_IN_THE_LIBRARY),
	prevForm(prevForm_), printBookForm(new GetPrintBookForm)
{
	ui->setupUi(this);
	setInterfaceStyle();

	connect(ui->printBookBtn, &QPushButton::clicked, this, &CheckBookForm::printBtnClickHandler);

	connect(printBookForm->ui->confirmBtn, &QPushButton::clicked, this, [=]() {
		if (reservPrintBook()) {
			resetForm(prevForm, userId, bookId);
		}
		});

	connect(ui->audioBookBtn, &QPushButton::clicked, this, [=]() {
		if (audioBook == BookFormatStatus::USER_HAVE) {
			
		}
		else if (giveBookToUser(BookFormat::AUDIOBOOK)) {
			resetForm(prevForm, userId, bookId);
		}
		});
	connect(ui->eBookBtn, &QPushButton::clicked, this, [=]() {
		if (eBook == BookFormatStatus::USER_HAVE) {
			openEBook();
		}
		else if (giveBookToUser(BookFormat::E_BOOK)) {
			resetForm(prevForm, userId, bookId);
		}
		});
	connect(ui->audioBookRetBtn, &QPushButton::clicked, this, [=]() {
		if (returnBook(BookFormat::AUDIOBOOK)) {
			resetForm(prevForm, userId, bookId);
		}
		});
	connect(ui->eBookRetBtn, &QPushButton::clicked, this, [=]() {
		if (returnBook(BookFormat::E_BOOK)) {
			resetForm(prevForm, userId, bookId);
		}
		});

	//resetForm(prevForm,userId,bookId); расскоментируем эту строчку ,если запускаем это окно отдельно
		
}

void CheckBookForm::resetForm(NavigationIndexes prevForm, int userId, int bookId) noexcept
{
	this->userId = userId;
	this->bookId = bookId;
	this->prevForm = prevForm;

	ui->audioBookHint->setText("");
	ui->eBookHint->setText("");
	ui->printBookHint->setText("");

	try {
		getBookInformationFromDB();
	}
	catch (const sql::SQLException& ex) {
		ui->title->setText("Произошла ошибка, пожалуйста вернитесь позднее");
		std::cerr << "problem with connection in CheckBookForm::resetForm" << std::endl;
	}

	this->setWindowTitle(ui->bookName->text());
}


void CheckBookForm::getBookInformationFromDB()
{
	try {
		std::string strBookInform =
			"SELECT books.name as name, GROUP_CONCAT("
			"IF(surname = '',"
			"CONCAT(first_name, ' ', last_name),"
			"CONCAT(SUBSTRING(first_name, 1, 1), '.', SUBSTRING(surname, 1, 1), '. ', authors.last_name)) "
			"SEPARATOR ', ') AS author, full_descript, "
			"year_publication ,image "
			"FROM books "
			"LEFT JOIN books_authors ON books.id = books_authors.book_id "
			"LEFT JOIN authors ON authors.id = books_authors.author_id "
			"WHERE books.id = " + std::to_string(bookId);

		std::unique_ptr<sql::PreparedStatement> pstmtBookInform(conn->prepareStatement(strBookInform));
		std::unique_ptr<sql::ResultSet> resBookInform(pstmtBookInform->executeQuery());
		std::string image;

		if (resBookInform->next()) {
			ui->bookName->setText(QString::fromUtf8(std::string(resBookInform->getString("name"))));
			ui->bookAuthor->setText(QString::fromUtf8(std::string(resBookInform->getString("author"))));
			ui->bookYear->setText(QString::fromUtf8(std::string(resBookInform->getString("year_publication"))));
			ui->bookDescription->setText(QString::fromUtf8(std::string(resBookInform->getString("full_descript"))));
			image = resBookInform->getString("image");
		}

		QPixmap bookImage;
		bookImage.load(QString::fromUtf8(image));
		ui->bookCover->setPixmap(bookImage);

		std::string strBookGenre =
			"SELECT GROUP_CONCAT(genre SEPARATOR ', ') AS genre "
			"FROM books_genres "
			"LEFT JOIN genres ON genres.id = books_genres.genre_id "
			"WHERE books_genres.book_id = " + std::to_string(bookId);
		std::unique_ptr<sql::PreparedStatement> pstmtBookGenre(conn->prepareStatement(strBookGenre));
		std::unique_ptr<sql::ResultSet> resBookGenre(pstmtBookGenre->executeQuery());
		if (resBookGenre->next()) {
			ui->bookGenres->setText(QString::fromUtf8(std::string(resBookGenre->getString("genre"))));
		}

		// Проверяем есть ли у пользователя эта книга ,или какой статус у этой книги в библиотеке

		for (int i = 0; i < 3; ++i) {
			std::string bookType = (i == 0) ? "Печатный формат" : (i == 1) ? "Электронная версия" : "Аудиокнига";
			QLabel* hint = (i == 0) ? ui->printBookHint : (i == 1) ? ui->eBookHint : ui->audioBookHint;
			hint->setWordWrap(true);
			int bookTypeId = getBookTypeId(bookType);
			std::string strDaysBookWillWithUser =
				"SELECT days - TIMESTAMPDIFF(DAY,date_taken,CURDATE()) as cnt_days FROM users_books "
				"LEFT JOIN books_types ON books_types.id = users_books.book_type_id "
				"LEFT JOIN books ON books_types.book_id = books.id "
				"WHERE user_id = " + std::to_string(userId) + " AND book_type_id = " + std::to_string(bookTypeId) + " AND date_return IS NULL AND date_taken IS NOT NULL";
			std::unique_ptr<sql::PreparedStatement> pstmtDaysBookWillWithUser(conn->prepareStatement(strDaysBookWillWithUser));
			std::unique_ptr<sql::ResultSet> resDaysBookWillWithUser(pstmtDaysBookWillWithUser->executeQuery());

			std::string strIsBookReserved =
				"SELECT users_books.id as id FROM users_books "
				"LEFT JOIN books_types ON books_types.id = users_books.book_type_id "
				"LEFT JOIN books ON books_types.book_id = books.id "
				"WHERE user_id = " + std::to_string(userId) + " AND book_type_id = " + std::to_string(bookTypeId) + " AND date_return IS NULL AND date_taken IS NULL";
			std::unique_ptr<sql::PreparedStatement> pstmtIsBookReserved(conn->prepareStatement(strIsBookReserved));
			std::unique_ptr<sql::ResultSet> resIsBookReserved(pstmtIsBookReserved->executeQuery());

			BookFormatStatus bookStatus;
			if (resDaysBookWillWithUser->next()) {
				int cnt_days = resDaysBookWillWithUser->getInt("cnt_days");
				if (cnt_days >= 0) {
					std::string dayForm = (cnt_days % 10 == 1) ? "день" : (cnt_days % 10 >= 2 && cnt_days % 10 <= 4) ? "дня" : "дней";
					std::string message = "Книга достуна еще " + std::to_string(cnt_days) + " " + dayForm;
					hint->setText(QString::fromUtf8(message));
				}
				else {
					hint->setText("Период на который выдавалась книга закончился. Пожалуйста верните книгу в библиотеку");
				}
				bookStatus = BookFormatStatus::USER_HAVE;
			}
			else if (resIsBookReserved->next()) {
				bookStatus = BookFormatStatus::USER_RESERVED;
			}
			else {
				std::string strDateWhenUserHadBook =
					"SELECT DATE_FORMAT(date_taken,'%d.%m.%Y') AS date_taken,DATE_FORMAT(date_return,'%d.%m.%Y') AS date_return "
					"FROM users_books "
					"WHERE user_id = " + std::to_string(userId) + " AND book_type_id = " + std::to_string(bookTypeId) + " AND date_return IS NOT NULL "
					"ORDER BY date_return DESC LIMIT 1";
				std::unique_ptr<sql::PreparedStatement> pstmtDateWhenUserHadBook(conn->prepareStatement(strDateWhenUserHadBook));
				std::unique_ptr<sql::ResultSet> resDateWhenUserHadBook(pstmtDateWhenUserHadBook->executeQuery());

				if (resDateWhenUserHadBook->next()) {
					std::string date_taken = resDateWhenUserHadBook->getString("date_taken");
					std::string date_return = resDateWhenUserHadBook->getString("date_return");
					std::string message = "Вы брали книгу в период с " + date_taken + " по " + date_return;
					hint->setText(QString::fromUtf8(message));
				}

				std::string strIsBookInStock =
					"SELECT in_stock FROM books_types "
					"WHERE type = '" + bookType + "' AND book_id = " + std::to_string(bookId);
				std::unique_ptr<sql::PreparedStatement> pstmtIsBookInStock(conn->prepareStatement(strIsBookInStock));
				std::unique_ptr<sql::ResultSet> resIsBookInStock(pstmtIsBookInStock->executeQuery());

				if (resIsBookInStock->next()) {

					if (resIsBookInStock->getInt("in_stock") > 0) {
						bookStatus = BookFormatStatus::IN_STOCK;
					}
					else {
						bookStatus = BookFormatStatus::ENDED;
					}

				}
				else {
					bookStatus = BookFormatStatus::NOT_IN_THE_LIBRARY;
				}
			}
			if (i == 0) {
				printBook = bookStatus;
			}
			else if (i == 1) {
				eBook = bookStatus;
			}
			else {
				audioBook = bookStatus;
			}
		}

		// книги добавлены в вектор в том же порядке что они находятся в окне , 
		// это важно для строк по типу ui->typesBookLayout->addWidget(buttons[i].hint, i, 2, 1, 2) , чтобы мы вернули подсказку на прежнюю позицию(с раширением позиции)
		std::vector<TypeBookInform> buttons = { TypeBookInform(audioBook,"Аудиокнига",ui->audioBook ,ui->audioBookBtn,ui->audioBookRetBtn,ui->audioBookHint),
												TypeBookInform(eBook,"Электронная версия",ui->eBook ,ui->eBookBtn,ui->eBookRetBtn,ui->eBookHint),
										TypeBookInform(printBook,"Печатный формат",ui->printBook ,ui->printBookBtn,ui->printBookRetBtn,ui->printBookHint) };
		for (int i = 0; i < 3; ++i) {

			buttons[i].bookRetBtn->hide(); // прячем кнопку возвращения книги и вернем ее обратно ,только если книга уже есть у пользователя
			ui->typesBookLayout->removeWidget(buttons[i].hint);   // сдвигаем подсказку на место кнопки которую скрыли
			ui->typesBookLayout->addWidget(buttons[i].hint, i, 2, 1, 2); // сдвигаем подсказку на место кнопки которую скрыли

			if (buttons[i].status == BookFormatStatus::NOT_IN_THE_LIBRARY) {
				buttons[i].bookInform->setText(buttons[i].type + ": нет в библиотеке");
				buttons[i].bookBtn->hide();
			}
			else if (buttons[i].status == BookFormatStatus::ENDED) {
				buttons[i].bookInform->setText(buttons[i].type + ": разобрали");
				buttons[i].bookBtn->hide();
			}
			else if (buttons[i].status == BookFormatStatus::IN_STOCK) {
				buttons[i].bookInform->setText(buttons[i].type + ": в наличии");
				buttons[i].bookBtn->setText("Получить");
				buttons[i].bookBtn->show();
			}
			else if (buttons[i].status == BookFormatStatus::USER_RESERVED) {
				buttons[i].bookInform->setText(buttons[i].type + ": забронирован");
				buttons[i].bookBtn->setText("Информация");
				buttons[i].bookBtn->show();
			}
			else if (buttons[i].status == BookFormatStatus::USER_HAVE && i == 2) { // i == 2 значит книга печатного формата
				buttons[i].bookInform->setText(buttons[i].type + ": уже у вас");
				buttons[i].bookBtn->setText("Информация");
				buttons[i].bookBtn->show();
			}
			else if (buttons[i].status == BookFormatStatus::USER_HAVE) {
				buttons[i].bookInform->setText(buttons[i].type + ": уже у вас");
				buttons[i].bookBtn->setText("Открыть");
				buttons[i].bookBtn->show();
				ui->typesBookLayout->removeWidget(buttons[i].hint);
				ui->typesBookLayout->addWidget(buttons[i].hint, i, 3, 1, 1);
				buttons[i].bookRetBtn->show();
			}
		}
	}
	catch (const sql::SQLException& ex) {
		ui->title->setText("Произошла ошибка, пожалуйста вернитесь позднее");
		std::cerr << "problem with connection in CheckBookForm::CheckBookForm connect(ui->printBookBtn, &QPushButton::clicked, this, [=]() {..." << std::endl;
	}

}


void CheckBookForm::printBtnClickHandler() noexcept {
	try {
		if (printBook == BookFormatStatus::USER_HAVE) {

			int printBookId = getBookTypeId("Печатный формат");
			std::pair<std::string, std::string> deadlineAndCode = getDeadlineTakenAndCode(printBookId); //вовзращает число до которого нужно сдать книгу и код пользователя
			printBookForm->ui->retBookDate->setText(QString::fromUtf8(deadlineAndCode.first));
			printBookForm->ui->retUserCode->setText(QString::fromUtf8(deadlineAndCode.second));
			printBookForm->ui->stackedWidget->setCurrentIndex(static_cast<int>(PrintBookFormIndexes::RETURN_FORM));
			printBookForm->show();
		}
		else if (printBook == BookFormatStatus::USER_RESERVED) {

			int printBookId = getBookTypeId("Печатный формат");
			std::pair<std::string, std::string> deadlineAndCode = getDeadlineReservAndCode(printBookId); //вовзращает число до которого нужно забронирована книга и код пользователя
			printBookForm->ui->reservationDateSuccess->setText(QString::fromUtf8(deadlineAndCode.first));
			printBookForm->ui->userCode->setText(QString::fromUtf8(deadlineAndCode.second));
			printBookForm->ui->stackedWidget->setCurrentIndex(static_cast<int>(PrintBookFormIndexes::SUCCESS_FORM));
			printBookForm->show();

		}
		else {
			printBookForm->ui->stackedWidget->setCurrentIndex(static_cast<int>(PrintBookFormIndexes::MAIN_FORM));
			printBookForm->show();
		}
	}
	catch (const sql::SQLException& ex) {
		ui->printBookHint->setText("Произошла ошибка, пожалуйста вернитесь позднее");
		std::cerr << "problem with connection in CheckBookForm::getBookInformationFromDB" << std::endl;
	}
}


void CheckBookForm::openEBook() noexcept
{
	try {
		std::string strBookFilePath = "SELECT file_path FROM books_types WHERE book_id = ? AND type = 'Электронная версия'";
		std::unique_ptr<sql::PreparedStatement> pstmtBookFilePath(conn->prepareStatement(strBookFilePath));
		pstmtBookFilePath->setInt(1, bookId);
		std::unique_ptr<sql::ResultSet> resBookFilePath(pstmtBookFilePath->executeQuery());
		if (resBookFilePath->next()) {
			QString file_path(QString::fromUtf8(std::string(resBookFilePath->getString("file_path"))));
			QString bookName = ui->bookName->text();
			emit openEBookSignal(file_path, bookName);
		}
	}
	catch (const sql::SQLException& ex) {
		ui->eBookHint->setText("Произошла ошибка");
		std::cerr << "problem with connection in  CheckBookForm::openEBook" << std::endl;
	}

}


bool CheckBookForm::giveBookToUser(BookFormat format) noexcept
{
	QLabel* hint = ui->audioBookHint; // временное значение, действительное узнается ниже в if(ах)
	std::unique_ptr<sql::Statement> stmtIsolLevel(conn->createStatement());
	try {
		std::string bookType;
		if (format == BookFormat::AUDIOBOOK) {
			bookType = "Аудиокнига";
			hint = ui->audioBookHint;
		}
		else if (format == BookFormat::E_BOOK) {
			bookType = "Электронная версия";
			hint = ui->eBookHint;
		}

		int bookTypeId = getBookTypeId(bookType);

		stmtIsolLevel->execute("SET SESSION TRANSACTION ISOLATION LEVEL SERIALIZABLE");
		conn->setAutoCommit(false);

		if (checkErrors(bookTypeId, hint)) {
			conn->commit();
			conn->setAutoCommit(true);
			stmtIsolLevel->execute("SET SESSION TRANSACTION ISOLATION LEVEL READ COMMITTED");
			return false;
		}
		else {
			std::string strRemBookFromLib = "UPDATE books_types SET in_stock = in_stock - 1 WHERE id = ?";
			std::unique_ptr<sql::PreparedStatement> pstmtRemBookFromLib(conn->prepareStatement(strRemBookFromLib));
			pstmtRemBookFromLib->setInt(1, bookTypeId);
			pstmtRemBookFromLib->executeUpdate();

			std::string strAddBookToUser = "INSERT INTO users_books (user_id,book_type_id,date_taken) VALUES(?,?,CURDATE())";
			std::unique_ptr<sql::PreparedStatement> pstmtAddBookToUser(conn->prepareStatement(strAddBookToUser));
			pstmtAddBookToUser->setInt(1, userId);
			pstmtAddBookToUser->setInt(2, bookTypeId);
			pstmtAddBookToUser->execute();

			conn->commit();
			conn->setAutoCommit(true);
			stmtIsolLevel->execute("SET SESSION TRANSACTION ISOLATION LEVEL READ COMMITTED");
			return true;
		}

	}
	catch (const sql::SQLException& ex) {
		conn->rollback();
		conn->setAutoCommit(true);
		stmtIsolLevel->execute("SET SESSION TRANSACTION ISOLATION LEVEL READ COMMITTED");
		hint->setText("Произошла ошибка при добавлении книги");
		std::cerr << "problem with connection in CheckBookForm::giveBookToUser" << std::endl;
		return false;
	}
}

bool CheckBookForm::reservPrintBook() noexcept
{
	std::unique_ptr<sql::Statement> stmtIsolLevel(conn->createStatement());
	try {
		std::string bookType;

		int bookTypeId = getBookTypeId("Печатный формат");
		std::string orderCode = getCodeForBookOrder();

		stmtIsolLevel->execute("SET SESSION TRANSACTION ISOLATION LEVEL SERIALIZABLE");
		conn->setAutoCommit(false);


		if (checkErrors(bookTypeId, printBookForm->ui->errorText)) {
			printBookForm->ui->stackedWidget->setCurrentIndex(static_cast<int>(PrintBookFormIndexes::ERROR_FORM));
			conn->commit();
			conn->setAutoCommit(true);
			stmtIsolLevel->execute("SET SESSION TRANSACTION ISOLATION LEVEL READ COMMITTED");
			return false;
		}
		else {
			printBookForm->ui->stackedWidget->setCurrentIndex(static_cast<int>(PrintBookFormIndexes::MAIN_FORM));
			std::string strRemBookFromLib = "UPDATE books_types SET in_stock = in_stock - 1 WHERE id = ?";
			std::unique_ptr<sql::PreparedStatement> pstmtRemBookFromLib(conn->prepareStatement(strRemBookFromLib));
			pstmtRemBookFromLib->setInt(1, bookTypeId);
			pstmtRemBookFromLib->executeUpdate();

			std::string strAddBookToUser = "INSERT INTO users_books (user_id,book_type_id,date_reserv,code) VALUES(?,?,CURDATE(),?)";
			std::unique_ptr<sql::PreparedStatement> pstmtAddBookToUser(conn->prepareStatement(strAddBookToUser));

			pstmtAddBookToUser->setInt(1, userId);
			pstmtAddBookToUser->setInt(2, bookTypeId);
			pstmtAddBookToUser->setString(3, orderCode);
			pstmtAddBookToUser->execute();

			conn->commit();
			conn->setAutoCommit(true);
			stmtIsolLevel->execute("SET SESSION TRANSACTION ISOLATION LEVEL READ COMMITTED");

			std::pair<std::string, std::string> deadlineAndCode = getDeadlineReservAndCode(bookTypeId);
			printBookForm->ui->reservationDateSuccess->setText(QString::fromUtf8(deadlineAndCode.first));
			printBookForm->ui->userCode->setText(QString::fromUtf8(deadlineAndCode.second));
			printBookForm->ui->stackedWidget->setCurrentIndex(static_cast<int>(PrintBookFormIndexes::SUCCESS_FORM));
		
			return true;
		}
	}
	catch (const sql::SQLException& ex) {
		conn->rollback();
		conn->setAutoCommit(true);
		stmtIsolLevel->execute("SET SESSION TRANSACTION ISOLATION LEVEL READ COMMITTED");
		printBookForm->ui->errorText->setText("Произошла ошибка при добавлении книги");
		std::cerr << "problem with connection in CheckBookForm::reservPrintBook" << std::endl;
		return false;
	}
}

bool CheckBookForm::returnBook(BookFormat format)
{
	QLabel* hint; 
	std::unique_ptr<sql::Statement> stmtIsolLevel(conn->createStatement());
	try {
		std::string bookType;
		if (format == BookFormat::AUDIOBOOK) {
			bookType = "Аудиокнига";
			hint = ui->audioBookHint;
		}
		else if (format == BookFormat::E_BOOK) {
			bookType = "Электронная версия";
			hint = ui->eBookHint;
		}
		int bookTypeId = getBookTypeId(bookType);

		stmtIsolLevel->execute("SET SESSION TRANSACTION ISOLATION LEVEL SERIALIZABLE");
		conn->setAutoCommit(false);

		std::string strDelUserBook = "UPDATE users_books SET date_return = CURDATE() WHERE book_type_id = ? AND user_id = ? AND date_return IS NULL";
		std::unique_ptr<sql::PreparedStatement> pstmtDelUserBook(conn->prepareStatement(strDelUserBook));
		pstmtDelUserBook->setInt(1, bookTypeId);
		pstmtDelUserBook->setInt(2, userId);
		pstmtDelUserBook->execute();

		std::string strAddBookInLib = "UPDATE books_types SET in_stock = in_stock + 1 WHERE book_id = ? AND type = ?";
		std::unique_ptr<sql::PreparedStatement> pstmtAddBookInLib(conn->prepareStatement(strAddBookInLib));
		pstmtAddBookInLib->setInt(1, bookId);
		pstmtAddBookInLib->setString(2, bookType);
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
		hint->setText("Произошла ошибка");
		std::cerr << "problem with connection in CheckBookForm::returnBook" << std::endl;
		return false;
	}

}

bool CheckBookForm::checkErrors(int bookTypeId,QLabel* hint) noexcept
{
	try {
		int cntUserBooks = getCntUserBooks();
		bool isBookAvailab = getCheckBookAvailab(bookTypeId);

		std::pair<int, int> informUserBookRecently = getCheckBookUserRecently(bookTypeId);
		bool checkBookUserRecently = informUserBookRecently.first;
		bool haveUserDebt = getHaveUserDebt();
		int remainderDays = informUserBookRecently.second;

		if (checkBookUserRecently == true) {
			std::string warning = "Вы можете взять эту книгу еще раз только спустя 30 дней после возвращения \n";
			std::string dayForm = (remainderDays % 10 == 1) ? "день" : (remainderDays % 10 >= 2 && remainderDays % 10 <= 4) ? "дня" : "дней";
			std::string full_hint = warning + "Книга будет доступна вам через " + std::to_string(remainderDays) + " " + dayForm;
			hint->setText(QString::fromUtf8(full_hint));
		}
		else if (haveUserDebt) {
			hint->setText("У вас есть невозвращенная вовремя книга");
		}
		else if (cntUserBooks == 3) {
			printBookForm->ui->stackedWidget->setCurrentIndex(static_cast<int>(PrintBookFormIndexes::ERROR_FORM));
			hint->setText("Пользователь не может брать более 3 книг");
		}
		else if (isBookAvailab == false) {
			printBookForm->ui->stackedWidget->setCurrentIndex(static_cast<int>(PrintBookFormIndexes::ERROR_FORM));
			hint->setText("К сожалению все книги разобраны");
		}
		else {
			return false;
		}
		return true;
	}
	catch (const sql::SQLException& ex) {
		hint->setText("Произошла ошибка");
		std::cerr << "problem with connection in CheckBookForm::checkErrors" << std::endl;
		return true;
	}
}

int CheckBookForm::getCntUserBooks() const 
{
	std::string strCntUserBooks = "SELECT COUNT(id) AS cnt FROM users_books WHERE user_id = ? AND date_return IS NULL";
	std::unique_ptr<sql::PreparedStatement> pstmtCntUserBooks(conn->prepareStatement(strCntUserBooks));
	pstmtCntUserBooks->setInt(1, userId);
	std::unique_ptr<sql::ResultSet> resCntUserBooks(pstmtCntUserBooks->executeQuery());
	int cntUserBooks = 0;
	if (resCntUserBooks->next()) {
		cntUserBooks = resCntUserBooks->getInt("cnt");
	}
	return cntUserBooks;
}

std::string CheckBookForm::getCodeForBookOrder() const 
{
	bool isCodeUnique = false;
	while (!isCodeUnique) {

		srand(time(NULL));
		int code = rand() % 1000000;
		std::string strCode = std::to_string(code);

		while (strCode.size() != 6) {
			strCode = "0" + strCode;
		}

		std::string strCheckUniqueCode = "SELECT code FROM users_books WHERE code = ?";
		std::unique_ptr<sql::PreparedStatement> pstmtCheckUniqueCode(conn->prepareStatement(strCheckUniqueCode));
		pstmtCheckUniqueCode->setString(1, strCode);

		std::unique_ptr<sql::ResultSet> resCheckUniqueCode(pstmtCheckUniqueCode->executeQuery());
		if (resCheckUniqueCode->next()) {
			isCodeUnique = false;
		}
		else {
			return strCode;
		}

	}

}

bool CheckBookForm::getCheckBookAvailab(int bookTypeId) const 
{
	std::string strCheckBookAvailab = "SELECT in_stock FROM books_types WHERE id = ?";
	std::unique_ptr<sql::PreparedStatement> pstmtCheckBookAvailab(conn->prepareStatement(strCheckBookAvailab));
	pstmtCheckBookAvailab->setInt(1, bookTypeId);
	std::unique_ptr<sql::ResultSet> resCheckBookAvailab(pstmtCheckBookAvailab->executeQuery());
	bool isBookAvailab = false;
	if (resCheckBookAvailab->next()) {
		isBookAvailab = resCheckBookAvailab->getInt("in_stock");
	}
	return isBookAvailab;
}

std::pair<bool, int> CheckBookForm::getCheckBookUserRecently(int bookTypeId) const 
{
	std::string strCheckBookUserRecently = "SELECT id, 30 - TIMESTAMPDIFF(DAY,date_return,CURDATE()) AS remainder "
		" FROM users_books "
		" WHERE user_id = ? AND book_type_id = ? AND "
		" date_return IS NOT NULL AND date_return NOT BETWEEN CURDATE() AND SUBDATE(CURDATE(), 30) ";
	std::unique_ptr<sql::PreparedStatement> pstmtCheckBookUserRecently(conn->prepareStatement(strCheckBookUserRecently));
	pstmtCheckBookUserRecently->setInt(1, userId);
	pstmtCheckBookUserRecently->setInt(2, bookTypeId);
	std::unique_ptr<sql::ResultSet> resCheckBookUserRecently(pstmtCheckBookUserRecently->executeQuery());

	bool checkBookUserRecently = false;
	int remainderDays = 0;
	if (resCheckBookUserRecently->next()) {
		checkBookUserRecently = true;
		remainderDays = resCheckBookUserRecently->getInt("remainder");
	}

	return std::make_pair(checkBookUserRecently, remainderDays);
}

std::pair<std::string,std::string> CheckBookForm::getDeadlineReservAndCode(int bookTypeId) const
{
	std::string strDeadlineReservAndCode =
		"SELECT DATE_FORMAT(DATE_ADD(date_reserv,INTERVAL 8 DAY),'%d.%m.%Y') AS date_taken,code "
		"FROM users_books "
		"WHERE user_id = " + std::to_string(userId) + " AND book_type_id = " + std::to_string(bookTypeId) + " AND date_return IS NULL AND date_taken is NULL";
	std::unique_ptr<sql::PreparedStatement> pstmtDeadlineReservAndCode(conn->prepareStatement(strDeadlineReservAndCode));
	std::unique_ptr<sql::ResultSet> resDeadlineReservAndCode(pstmtDeadlineReservAndCode->executeQuery());
	std::string deadline;
	std::string code;
	if (resDeadlineReservAndCode->next()) {
		deadline = resDeadlineReservAndCode->getString("date_taken");
		code = resDeadlineReservAndCode->getString("code");
	}
	return std::make_pair(deadline, code);
}

std::pair<std::string, std::string> CheckBookForm::getDeadlineTakenAndCode(int bookTypeId) const 
{
	std::string strDeadlineTakenAndCode =
		"SELECT DATE_FORMAT(DATE_ADD(date_taken, INTERVAL days DAY), '%d.%m.%Y') AS date_return, code "
		"FROM users_books "
		"LEFT JOIN books_types ON users_books.book_type_id = books_types.id "
		"LEFT JOIN books ON books.id = books_types.book_id "
		"WHERE user_id = ? AND book_type_id = ? AND date_return IS NULL AND date_taken IS NOT NULL ";
	std::unique_ptr<sql::PreparedStatement> pstmtDeadlineTakenAndCode(conn->prepareStatement(strDeadlineTakenAndCode));
	pstmtDeadlineTakenAndCode->setInt(1, userId);
	pstmtDeadlineTakenAndCode->setInt(2, bookTypeId);
	std::unique_ptr<sql::ResultSet> resDeadlineTakenAndCode(pstmtDeadlineTakenAndCode->executeQuery());
	std::string deadline;
	std::string code;
	if (resDeadlineTakenAndCode->next()) {
		deadline = resDeadlineTakenAndCode->getString("date_return");
		code = resDeadlineTakenAndCode->getString("code");
	}
	return std::make_pair(deadline, code);
}

int CheckBookForm::getBookTypeId(std::string bookType) const
{
	std::string strBookTypeId = "SELECT id FROM books_types WHERE book_id = ? AND type = ?";
	std::unique_ptr<sql::PreparedStatement> pstmtBookTypeId(conn->prepareStatement(strBookTypeId));
	pstmtBookTypeId->setInt(1, bookId);
	pstmtBookTypeId->setString(2, bookType);
	std::unique_ptr<sql::ResultSet> resBookTypeId(pstmtBookTypeId->executeQuery());
	int bookTypeId = 0;
	if (resBookTypeId->next()) {
		bookTypeId = resBookTypeId->getInt("id");
	}
	return bookTypeId;
}

bool CheckBookForm::getHaveUserDebt() const 
{
	std::string strHaveUserDebt = "SELECT users_books.id FROM users_books "
		"LEFT JOIN books_types ON books_types.id = users_books.book_type_id "
		"LEFT JOIN books ON books.id = books_types.book_id "
		"WHERE DATE_ADD(date_taken, INTERVAL days DAY) < CURDATE() AND date_return IS NULL AND users_books.user_id = ? ";
	std::unique_ptr<sql::PreparedStatement> pstmtHaveUserDebt(conn->prepareStatement(strHaveUserDebt));
	pstmtHaveUserDebt->setInt(1, userId);
	std::unique_ptr<sql::ResultSet> resHaveUserDebt(pstmtHaveUserDebt->executeQuery());
	bool HaveUserDebt = false;
	if (resHaveUserDebt->next()) {
		HaveUserDebt = true;
	}
	return HaveUserDebt;
}

void CheckBookForm::resizeEvent(QResizeEvent* event) noexcept
{
	//std::cout << "width: " << this->width() << std::endl;
	//std::cout << "height: " << this->height() << std::endl;

	if (this->height() <= 670) {
		ui->headerLayout->setContentsMargins(3, 3, 3, 3);
		ui->mainWidgetLayout->setContentsMargins(ui->mainWidgetLayout->contentsMargins().left(), 5, ui->mainWidget->contentsMargins().right(), 5);
	}
	else if (this->height() > 670) {
		ui->headerLayout->setContentsMargins(9, 9, 9, 9);
		ui->mainWidgetLayout->setContentsMargins(ui->mainWidgetLayout->contentsMargins().left(), 35, ui->mainWidgetLayout->contentsMargins().right(), 10);
	}

	if (this->width() <= 950 || this->height() <= 705) {
		ui->bookCover->setMinimumSize(QSize(250, 350));
		ui->bookCover->setMaximumSize(QSize(250, 350));
		ui->mainWidgetLayout->setContentsMargins(10, ui->mainWidgetLayout->contentsMargins().top(), 10, ui->mainWidgetLayout->contentsMargins().bottom());
		ui->mainWidget->setStyleSheet("border: none");
		QFont font;
		font.setPointSize(10);
		ui->bookDescription->setFont(font);
		ui->mainWidgetLayout->setHorizontalSpacing(6);
	}
	else if (this->width() <= 1150) {
		ui->bookCover->setMinimumSize(QSize(320, 450));
		ui->bookCover->setMaximumSize(QSize(320, 450));
		ui->mainWidgetLayout->setContentsMargins(10, ui->mainWidgetLayout->contentsMargins().top(), 10, ui->mainWidgetLayout->contentsMargins().bottom());
		ui->mainWidget->setStyleSheet("border: none");
		QFont font;
		font.setPointSize(12);
		ui->bookDescription->setFont(font);
		ui->mainWidgetLayout->setHorizontalSpacing(15);
	}
	else if (this->width() > 1200) {
		ui->bookCover->setMinimumSize(QSize(320, 450));
		ui->bookCover->setMaximumSize(QSize(320, 450));
		ui->mainWidgetLayout->setContentsMargins(130, ui->mainWidgetLayout->contentsMargins().top(), 130, ui->mainWidgetLayout->contentsMargins().bottom());
		ui->mainWidget->setStyleSheet(StyleHelper::getMainWidgetCheckBookFormStyle());
		QFont font;
		font.setPointSize(12);
		ui->bookDescription->setFont(font);
		ui->mainWidgetLayout->setHorizontalSpacing(15);
	}
}

void CheckBookForm::setInterfaceStyle() noexcept
{
	ui->centralwidget->setStyleSheet(StyleHelper::getCentralWidgetStyle());
	ui->header->setStyleSheet(StyleHelper::getHeaderStyle());

	ui->mainWidget->setStyleSheet(StyleHelper::getMainWidgetCheckBookFormStyle());
	ui->backBtn->setStyleSheet(StyleHelper::getBackBtnStyle());

	ui->eBookBtn->setStyleSheet(StyleHelper::getButtonsStyle());
	ui->printBookBtn->setStyleSheet(StyleHelper::getButtonsStyle());
	ui->audioBookBtn->setStyleSheet(StyleHelper::getButtonsStyle());

	ui->eBookRetBtn->setStyleSheet(StyleHelper::getButtonsStyle());
	ui->audioBookRetBtn->setStyleSheet(StyleHelper::getButtonsStyle());
}

CheckBookForm::~CheckBookForm() noexcept
{
	delete printBookForm;
	delete ui;
}

TypeBookInform::TypeBookInform(BookFormatStatus status,QString type, QLabel* bookInform, QPushButton* bookBtn,QPushButton* bookRetBtn,QLabel* hint)
{
	this->status = status;
	this->type = type;
	this->bookInform = bookInform;
	this->bookBtn = bookBtn;
	this->bookRetBtn = bookRetBtn;
	this->hint = hint;
}


