#include <cppconn/resultset.h> 
#include <cppconn/prepared_statement.h>
#include "LibBooksForm.h"
#include "BookWidget.h"
#include "StyleHelper.h"
#include "PixmapLabel.h"
#include "qpixmap.h"
#include "qscrollbar.h"

LibBooksForm::LibBooksForm(std::shared_ptr<sql::Connection> conn_,int userId, QWidget* parent) noexcept :
	BooksOutput(conn_,userId,parent),allBooks(), searchBooks()
{
	ui->title->setText("Книги библиотеки");

	connect(ui->searchLine, &QLineEdit::editingFinished, this, [=]() {
		if (ui->searchLine->text() != QString("")) {
			prevSearchText = ui->searchLine->text();
			isSearch = true;
			booksOutput();
		}
		});
	connect(ui->searchLine, &QLineEdit::textChanged, this, [=]() {
		if (ui->searchLine->text() == QString("") && prevSearchText != QString("")) { // срабатывает только при очищении поля поиска
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

	ui->historyBookBtn->hide();
	//resetForm(); // если запускать окно LibBooksForm по отдельности надо раскомментировать эту строку
}


void LibBooksForm::booksOutput() noexcept {
	
	bool isFirstOutput = (allBooks.size() == 0);
	bool isReOuputAllBooks = (!isFirstOutput && !isSearch && !isCheckBoxesActive);

	try {
		if (isFirstOutput || isSearch || isCheckBoxesActive) { //вызываем когда надо обратиться к базе данных (первое открытие окна ,
			std::string strGetCatalogBooks;							/// поисковой запрос , активные чекбоксы )
			if (isFirstOutput) { // при первом открытии окна
				strGetCatalogBooks = getStrLibBooks();
			}
			else {
				std::string searchWords = ui->searchLine->text().toUtf8().constData();
				strGetCatalogBooks = getStrSearchLibBooks(searchWords, ui->audioBookCheckBox->isChecked(), ui->printBookCheckBox->isChecked(), ui->eBookCheckBox->isChecked());
				//далее очищаем или скрываем старый вывод книг
				for (size_t i = 0; i < allBooks.size(); ++i) {
					allBooks[i]->hide();
				}
				searchBooks.resize(0);
			}
			std::unique_ptr<sql::PreparedStatement> pstmtGetCatalogBooks(conn->prepareStatement(strGetCatalogBooks));
			std::unique_ptr<sql::ResultSet> resGetCatalogBooks(pstmtGetCatalogBooks->executeQuery());
			int row = 1, column = 0;
			while (resGetCatalogBooks->next()) {

				auto book = createBookWidget(resGetCatalogBooks);

				ui->booksTableLayout->addWidget(book.get(), row, column, 1, 1); // добавляем виджет книги в окно
				if (isFirstOutput) {
					allBooks.push_back(std::move(book)); // добавляем все книги в массив ,чтобы потом был к ним быстрый доступ
				}
				else {
					searchBooks.push_back(std::move(book)); // добавляем последние книги полученные с помощью поиска или чекбоксов в массив ,чтобы потом был к ним быстрый доступ
				}
				column++;
				if (column == cntColumn) {
					column = 0;
					row++;
				}
			}
			if (isSearch || isCheckBoxesActive) { // При поиске может быть мало книг поэтому добавляем "пустые" книги для красивого вывода найденных книг в QGridLayout
				
				for (int i = 0; i < 15 - (row * cntColumn + column); ++i) {
					auto book = std::make_unique<EmptyBookWidget>(ui->booksTable);
					book->setObjectName("book");
					QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
					sizePolicy.setHeightForWidth(book->sizePolicy().hasHeightForWidth());
					book->setSizePolicy(sizePolicy);
					book->setMinimumSize(QSize(220, 352));
					book->setMaximumSize(QSize(220, 352));
					ui->booksTableLayout->addWidget(book.get(), row, column, 1, 1);

					searchBooks.push_back(std::move(book));

					column++;
					if (column == cntColumn) {
						column = 0;
						row++;
					}
				}		
			}
			
		}
		else if (isReOuputAllBooks) { // если надо повторно вывести все книги библиотеки
			int row = 1, column = 0;

			//далее очищаем старый вывод книг
			searchBooks.resize(0);

			for (size_t i = 0; i < allBooks.size(); ++i) {
				allBooks[i]->show();
			}
			// добавляем на экран уже сохранненные книги
			for (size_t i = 0; i < allBooks.size(); ++i) {
				ui->booksTableLayout->addWidget(allBooks[i].get(), row, column, 1, 1);
				column++;
				if (column == cntColumn) {
					column = 0;
					row++;
				}
			}

		}
		else { // если надо повторно вывести книги полученные при помощи поисковой строки или чекбоксов(при изменении размеров экрана)
			int row = 1, column = 0;
			for (size_t i = 0; i < searchBooks.size(); ++i) {
				ui->booksTableLayout->addWidget(searchBooks[i].get(), row, column, 1, 1);
				column++;
				if (column == cntColumn) {
					column = 0;
					row++;
				}
			}
		}
		
		ui->booksArea->verticalScrollBar()->setValue(0); // поднимает ползунок вверх
	}
	catch (const sql::SQLException& ex) {
		ui->title->setText("Произошла ошибка ,пожалуйста вернитесь позднее");
		std::cerr << "problem with connection in LibBooksForm::booksOutput" << std::endl;
	}

}

std::string LibBooksForm::getStrLibBooks() const 
{
	std::string strLibBooks =
		"SELECT books.id as id,books.name AS name, GROUP_CONCAT( "
		"IF(surname = '', "
		"CONCAT(first_name, ' ', last_name), "
		"CONCAT(SUBSTRING(first_name, 1, 1), '.', SUBSTRING(surname, 1, 1), '. ', last_name)) "
		"SEPARATOR ', ') AS author , image from books "
		"LEFT JOIN books_authors ON books.id = books_authors.book_id "
		"LEFT JOIN authors ON authors.id = books_authors.author_id "
		"GROUP BY books.name,books.id "
		"ORDER BY books.id ";
	return strLibBooks;
}

std::string LibBooksForm::getStrSearchLibBooks(std::string searchWords, bool audioBook, bool printBook, bool eBook) const 
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

	std::string strIdBooksWithGivenTypes =
		"SELECT books.id FROM books "
		"LEFT JOIN books_types ON books.id = books_types.book_id "
		"WHERE books_types.type = '" + strAudioBook + "' OR books_types.type = '" + strPrintBook + "' OR books_types.type = '" + strEBook + "' "
		"GROUP BY books.id ";
	std::string strIdBooksWithGivenAuthor = "SELECT id FROM( "
		"SELECT books.id as id, last_name, first_name, surname "
		"FROM authors "
		"LEFT JOIN books_authors ON authors.id = books_authors.author_id "
		"LEFT JOIN books ON books.id = books_authors.book_id "
		"WHERE MATCH(last_name, first_name, surname) AGAINST('" + searchWords + " ' IN BOOLEAN MODE) "
		") fts_on_authors ";

	std::string strLibBooks;

	std::string startLineText = "Книга,автор";

	if (searchWords != "" && searchWords != startLineText && (audioBook || printBook || eBook)) {
		strLibBooks = "SELECT books.id as id,name , GROUP_CONCAT(DISTINCT "
			"IF(surname = '', CONCAT(first_name, ' ', last_name), CONCAT(SUBSTRING(first_name, 1, 1), '.', SUBSTRING(surname, 1, 1), '. ', last_name)) SEPARATOR ', ') "
			"AS author ,image "
			"FROM books "
			"LEFT JOIN books_authors ON books.id = books_authors.book_id "
			"LEFT JOIN authors ON authors.id = books_authors.author_id "
			"WHERE (books.id IN( " + strIdBooksWithGivenAuthor + " ) OR MATCH(name) AGAINST('" + searchWords + " ' IN BOOLEAN MODE))"
			"AND books.id IN ( " + strIdBooksWithGivenTypes + " ) "
			"GROUP BY books.name, books.id "
			"ORDER BY books.id ";
	}
	else if (searchWords != "" && searchWords != startLineText) {
		strLibBooks = "SELECT books.id as id,name , GROUP_CONCAT(DISTINCT "
			"IF(surname = '', CONCAT(first_name, ' ', last_name), CONCAT(SUBSTRING(first_name, 1, 1), '.', SUBSTRING(surname, 1, 1), '. ', last_name)) SEPARATOR ', ') "
			"AS author ,image "
			"FROM books "
			"LEFT JOIN books_authors ON books.id = books_authors.book_id "
			"LEFT JOIN authors ON authors.id = books_authors.author_id "
			"WHERE books.id IN( " + strIdBooksWithGivenAuthor + " )"
			"OR MATCH(name) AGAINST('" + searchWords + " ' IN BOOLEAN MODE) "
			"GROUP BY books.name, books.id "
			"ORDER BY books.id ";
	}
	else if (audioBook || printBook || eBook) {
		strLibBooks = "SELECT books.id as id,name , GROUP_CONCAT(DISTINCT "
			"IF(surname = '', CONCAT(first_name, ' ', last_name), CONCAT(SUBSTRING(first_name, 1, 1), '.', SUBSTRING(surname, 1, 1), '. ', last_name)) SEPARATOR ', ') "
			"AS author ,image "
			"FROM books "
			"LEFT JOIN books_authors ON books.id = books_authors.book_id "
			"LEFT JOIN authors ON authors.id = books_authors.author_id "
			"WHERE books.id IN( " + strIdBooksWithGivenTypes + " )"
			"GROUP BY books.name, books.id "
			"ORDER BY books.id ";
	}
	return strLibBooks;

}


void LibBooksForm::resetForm() noexcept
{
	BooksOutput::resetForm();
	booksOutput();
}


void LibBooksForm::resizeEvent(QResizeEvent* event) noexcept
{
	BooksOutput::resizeEvent(event);
	if (changeCntColumn == true) {
		booksOutput();
	}
}

LibBooksForm::~LibBooksForm() noexcept
{
	allBooks.resize(0);
	searchBooks.resize(0);
}