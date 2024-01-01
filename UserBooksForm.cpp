#include <cppconn/resultset.h> 
#include <cppconn/prepared_statement.h>
#include "qpixmap.h"
#include "qscrollbar.h"
#include "qscrollbar.h"
#include "PixmapLabel.h"
#include "UserBooksForm.h"
#include "BookWidget.h"
#include "StyleHelper.h"

UserBooksForm::UserBooksForm(std::shared_ptr<sql::Connection> conn_,int userId, QWidget* parent) noexcept :
	BooksOutput(conn_,userId, parent),historyMode(false)
{
	connect(ui->searchLine, &QLineEdit::editingFinished, this, [=]() { 
		if (ui->searchLine->text() != QString("")) {
			prevSearchText = ui->searchLine->text();
			isSearch = true;
			booksOutput();
		}
		});
	connect(ui->searchLine, &QLineEdit::textChanged, this, [=]() {
		if (ui->searchLine->text() == QString("") && prevSearchText != QString("")) { // срабатывает только при полном очищении поля поиска
			isSearch = false;
			prevSearchText = QString("");
			booksOutput();
		}
		});

	connect(ui->audioBookCheckBox, &QCheckBox::stateChanged, this, [=]() {
		if ((ui->audioBookCheckBox->isChecked() || ui->printBookCheckBox->isChecked() || ui->eBookCheckBox->isChecked()) == false) {
			isCheckBoxesActive = false;
		}
		else {
			isCheckBoxesActive = true;
		}
		booksOutput();
		});

	connect(ui->eBookCheckBox, &QCheckBox::stateChanged, this, [=]() {
		if ((ui->audioBookCheckBox->isChecked() || ui->printBookCheckBox->isChecked() || ui->eBookCheckBox->isChecked()) == false) {
			isCheckBoxesActive = false;
		}
		else {
			isCheckBoxesActive = true;
		}
		booksOutput();
		});
	connect(ui->printBookCheckBox, &QCheckBox::stateChanged, this, [=]() {
		if ((ui->audioBookCheckBox->isChecked() || ui->printBookCheckBox->isChecked() || ui->eBookCheckBox->isChecked()) == false) {
			isCheckBoxesActive = false;
		}
		else {
			isCheckBoxesActive = true;
		}
		booksOutput();
		});
	connect(ui->historyBookBtn, &QPushButton::clicked, this, [=]() {
		if (historyMode == false) {
			historyMode = true;
			ui->historyBookBtn->setText("Посмотреть текущие книги");
			ui->title->setText("Архив книг");
		}
		else {
			historyMode = false;
			ui->historyBookBtn->setText("Посмотреть архив книг");
			ui->title->setText("Ваши книги");
		}
		booksOutput();
		});
	setInterfaceStyle();
}

void UserBooksForm::resetForm() noexcept
{
	BooksOutput::resetForm();
	historyMode = false;
	ui->title->setText("Ваши книги");
	ui->historyBookBtn->setText("Посмотреть архив книг");
	booksOutput();
}

void UserBooksForm::booksOutput() noexcept
{
	books.resize(0);
	try {
		std::string searchWords = ui->searchLine->text().toStdString();
		std::string strUserBooks = getStrUserBooks(searchWords, ui->audioBookCheckBox->isChecked(), ui->printBookCheckBox->isChecked(), ui->eBookCheckBox->isChecked() , historyMode);
		std::unique_ptr<sql::PreparedStatement> pstmtUserBooks(conn->prepareStatement(strUserBooks));
		std::unique_ptr<sql::ResultSet> resUserBooks(pstmtUserBooks->executeQuery());
		int row = 1, column = 0;
		while (resUserBooks->next()) {
			auto book = createBookWidget(resUserBooks);

			ui->booksTableLayout->addWidget(book.get(), row, column, 1, 1); // добавляем виджет книги в окно
			column++;
			if (column == cntColumn) {
				column = 0;
				row++;
			}
			books.push_back(std::move(book)); // передаем управление вектору,чтобы умный указатель не удалился
		}
		ui->booksArea->verticalScrollBar()->setValue(0);
		
		for (int i = 0; i < 15 - (row * cntColumn + column); ++i) { // добавляем "пустные" книги для красивого вывода
			auto book = std::make_unique<EmptyBookWidget>(ui->booksTable);
			book->setObjectName("book");
			QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			sizePolicy.setHeightForWidth(book->sizePolicy().hasHeightForWidth());
			book->setSizePolicy(sizePolicy);
			book->setMinimumSize(QSize(220, 352));
			book->setMaximumSize(QSize(220, 352));
			ui->booksTableLayout->addWidget(book.get(), row, column, 1, 1);

			column++;
			if (column == cntColumn) {
				column = 0;
				row++;
			}
			books.push_back(std::move(book));
		}
	}
	catch (const sql::SQLException& ex) {
		ui->title->setText("Произошла ошибка ,пожалуйста вернитесь позднее");
		std::cerr << "problem with connection in LibBooksForm::booksOutput" << std::endl;
	}


}

void UserBooksForm::resizeEvent(QResizeEvent* event) noexcept
{
	BooksOutput::resizeEvent(event);
	if (changeCntColumn == true) {
		booksOutput();
	}

}

std::string UserBooksForm::getStrUserBooks(std::string searchWords, bool audioBook, bool printBook, bool eBook,bool history) const noexcept
{
	std::string strAudioBook = "Аудиокнига";
	std::string strPrintBook = "Печатный формат";
	std::string strEBook = "Электронная версия";
	if (!audioBook) {
		strAudioBook = "";
	}
	if (!printBook) {
		strPrintBook = "";
	}
	if (!eBook) {
		strEBook = "";
	}

	std::string date_return;
	if (history == true) {
		date_return = " NOT NULL ";
	}
	else {
		date_return = " NULL ";
	}

	std::string strIdBooksWithGivenTypes = // этот запрос отличается от похожего запроса в LibBooksForm
		"SELECT books.id FROM books "
		"LEFT JOIN books_types ON books.id = books_types.book_id "
		"LEFT JOIN users_books ON users_books.book_type_id = books_types.id "
		"WHERE(books_types.type = '" + strAudioBook + "'  OR books_types.type = '" + strPrintBook + "' OR books_types.type = '" + strEBook + "') "
		"AND user_id = " + std::to_string(userId) + " AND date_return IS " + date_return;
		" GROUP BY books.id ";
	std::string strIdBooksWithGivenAuthor = "SELECT id FROM( "
		"SELECT books.id as id, last_name, first_name, surname "
		"FROM authors "
		"LEFT JOIN books_authors ON authors.id = books_authors.author_id "
		"LEFT JOIN books ON books.id = books_authors.book_id "
		"WHERE MATCH(last_name, first_name, surname) AGAINST('" + searchWords + " ' IN BOOLEAN MODE) "
		") fts_on_authors ";

	std::string strUserBooks;

	std::string startLineText = "Книга,автор";
	
	std::string whereInSelect;
	if (searchWords != "" && searchWords != startLineText && (audioBook || printBook || eBook)) {
		whereInSelect =
			" WHERE (books.id IN( " + strIdBooksWithGivenAuthor + " ) OR MATCH(name) AGAINST('" + searchWords + " ' IN BOOLEAN MODE)) "
			" AND books.id IN ( " + strIdBooksWithGivenTypes + " )  AND "
			" books_types.id in(SELECT book_type_id FROM users_books WHERE user_id = " + std::to_string(userId) + " AND date_return IS " + date_return + ") "
			" AND user_id = " + std::to_string(userId);
	}
	else if (searchWords != "" && searchWords != startLineText) {
		whereInSelect = 
			" WHERE (books.id IN( " + strIdBooksWithGivenAuthor + " ) OR MATCH(name) AGAINST('" + searchWords + " ' IN BOOLEAN MODE)) AND "
			" books_types.id in(SELECT book_type_id FROM users_books WHERE user_id = " + std::to_string(userId) + " AND date_return IS " + date_return + ") "
			" AND user_id = " + std::to_string(userId);
	}
	else if (audioBook || printBook || eBook) {
		whereInSelect = 
			" WHERE books.id IN ( " + strIdBooksWithGivenTypes + " )  AND"
			" books_types.id in(SELECT book_type_id FROM users_books WHERE user_id = " + std::to_string(userId) + " AND date_return IS " + date_return + ") "
			" AND user_id = " + std::to_string(userId);
	}
	else {
		whereInSelect =
			" WHERE books_types.id IN(SELECT book_type_id FROM users_books WHERE user_id = " + std::to_string(userId) + " AND date_return IS " + date_return + ") "
			" AND user_id = " + std::to_string(userId);
	}
	strUserBooks = 
			"SELECT DISTINCT books.id as id, books.name AS name, GROUP_CONCAT(DISTINCT "
			"IF(surname = '', "
			"CONCAT(first_name, ' ', last_name), "
			"CONCAT(SUBSTRING(first_name, 1, 1), '.', SUBSTRING(surname, 1, 1), '. ', last_name)) "
			"SEPARATOR ', ') AS author, image FROM books "
			"LEFT JOIN books_authors ON books.id = books_authors.book_id "
			"LEFT JOIN authors ON authors.id = books_authors.author_id "
			"LEFT JOIN books_types ON books.id = books_types.book_id "
			"LEFT JOIN users_books ON books_types.id = users_books.book_type_id "
			+ whereInSelect + 
			" GROUP BY users_books.id ";
	return strUserBooks;
	
}


void UserBooksForm::setInterfaceStyle() noexcept
{
	BooksOutput::setInterfaceStyle();
	ui->historyBookBtn->setStyleSheet(StyleHelper::getButtonsStyle());
}
