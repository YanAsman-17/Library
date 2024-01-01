#include <cppconn/resultset.h> 
#include <cppconn/prepared_statement.h>
#include "BooksOutput.h"
#include "BookWidget.h"
#include "StyleHelper.h"
#include "PixmapLabel.h"
#include "qpixmap.h"
#include "qscrollbar.h"

BooksOutput::BooksOutput(std::shared_ptr<sql::Connection> conn_,int userId_, QWidget* parent) noexcept: 
	QMainWindow(parent), ui(new Ui::BooksOutput),conn(conn_), userId(userId_), cntColumn(0), changeCntColumn(false),
	isSearchLineChange(false), prevSearchText(""), isSearch(false), isCheckBoxesActive(false)
{

	ui->setupUi(this);
	setInterfaceStyle();

	ui->searchLine->installEventFilter(this);
	ui->booksArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//resetForm()  //если запускать BooksOutput как отдельное окно надо раскомментировать эту строку
}


void BooksOutput::resetForm() noexcept
{
	ui->title->setFocus();
	ui->searchLine->setText("Книга,автор");
	ui->searchLine->setStyleSheet(StyleHelper::getInputLineStyle());

	isSearch = false;
	isSearchLineChange = false;
	isCheckBoxesActive = false;
	ui->audioBookCheckBox->setChecked(false);
	ui->printBookCheckBox->setChecked(false);
	ui->eBookCheckBox->setChecked(false);
}




std::unique_ptr<BookWidget> BooksOutput::createBookWidget(std::unique_ptr<sql::ResultSet>& resGetCatalogBooks) noexcept // создаем окошко книги 
{
	auto book = std::make_unique<BookWidget>(this, ui->booksTable);
	book->setCursor(Qt::PointingHandCursor);
	book->setObjectName("book");
	QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	sizePolicy.setHeightForWidth(book->sizePolicy().hasHeightForWidth());
	book->setSizePolicy(sizePolicy);
	book->setMinimumSize(QSize(220, 352));
	book->setMaximumSize(QSize(220, 352));

	QVBoxLayout* bookLayout = new QVBoxLayout(book.get());
	bookLayout->setSpacing(2);
	bookLayout->setObjectName("bookLayout");

	PixmapLabel* bookCover = new PixmapLabel(book.get());
	bookCover->setObjectName("bookCover");
	sizePolicy.setHeightForWidth(bookCover->sizePolicy().hasHeightForWidth());
	bookCover->setSizePolicy(sizePolicy);
	bookCover->setAlignment(Qt::AlignCenter);

	QPixmap pix;
	std::string pathPix = std::string(resGetCatalogBooks->getString("image"));
	pix.load(QString::fromUtf8(pathPix));
	bookCover->setPixmap(pix);

	bookLayout->addWidget(bookCover);

	QLabel* bookTitle = new QLabel(book.get());
	bookTitle->setObjectName("bookTitle");
	bookTitle->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
	bookTitle->setWordWrap(true);
	bookTitle->setText(QString::fromUtf8(std::string(resGetCatalogBooks->getString("name"))));
	bookTitle->setStyleSheet("font-size: 14px");

	bookLayout->addWidget(bookTitle);

	QLabel* bookAuthor = new QLabel(book.get());
	bookAuthor->setObjectName("bookAuthor");
	bookAuthor->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
	bookAuthor->setWordWrap(true);
	bookAuthor->setText(QString::fromUtf8(std::string(resGetCatalogBooks->getString("author"))));
	bookAuthor->setStyleSheet("font-size: 14px");

	bookLayout->addWidget(bookAuthor);

	book->setProperty("id", resGetCatalogBooks->getInt("id"));
	return book;
}

int BooksOutput::getUserId() const
{
	return userId;
}

void BooksOutput::resizeEvent(QResizeEvent* event) noexcept
{
	QMainWindow::resizeEvent(event);
	changeCntColumn = false;
	static bool isFiltersHide = false;
	int diffPix = 250;
	int startWidth = 750;
	if (QMainWindow::width() < startWidth && cntColumn != 2) {
		cntColumn = 2;
		changeCntColumn = true;
		ui->filters->hide();
		isFiltersHide = true;
	}
	else if (QMainWindow::width() >= startWidth + 0 * diffPix && QMainWindow::width() < startWidth + 1 * diffPix && (cntColumn != 3 || cntColumn == 3 && !isFiltersHide)) {
		cntColumn = 3;
		changeCntColumn = true;
		ui->filters->hide();
		isFiltersHide = true;
	}
	else if (QMainWindow::width() >= startWidth + 1 * diffPix && QMainWindow::width() < startWidth + 2 * diffPix && (cntColumn != 3 || cntColumn == 3 && isFiltersHide)) {
		cntColumn = 3;
		changeCntColumn = true;
		ui->filters->show();
		isFiltersHide = false;
	}
	else if (QMainWindow::width() >= startWidth + 2 * diffPix && QMainWindow::width() < startWidth + 3 * diffPix && cntColumn != 4) {
		cntColumn = 4;
		changeCntColumn = true;
		ui->filters->show();
		isFiltersHide = false;
	}
	else if (QMainWindow::width() >= startWidth + 3 * diffPix && QMainWindow::width() < startWidth + 4 * diffPix && cntColumn != 5) {
		cntColumn = 5;
		changeCntColumn = true;
		ui->filters->show();
		isFiltersHide = false;
	}
	else if (QMainWindow::width() >= startWidth + 4 * diffPix && QMainWindow::width() < startWidth + 5 * diffPix && cntColumn != 6) {
		cntColumn = 6;
		changeCntColumn = true;
		ui->filters->show();
		isFiltersHide = false;
	}
	else if (QMainWindow::width() >= startWidth + 5 * diffPix && cntColumn != 7) {
		cntColumn = 7;
		changeCntColumn = true;
		ui->filters->show();
		isFiltersHide = false;
	}

}

bool BooksOutput::eventFilter(QObject* object, QEvent* event) noexcept
{
	if (object == ui->searchLine && event->type() == QEvent::FocusIn && !isSearchLineChange) { // событие первого нажатия на поисковую строку
		ui->searchLine->setText("");
		ui->searchLine->setStyleSheet(StyleHelper::getChangedInputLineStyle());
		isSearchLineChange = true;
	}
	return false;
}


void BooksOutput::setInterfaceStyle() noexcept
{
	ui->centralwidget->setStyleSheet(StyleHelper::getCentralWidgetStyle());
	ui->header->setStyleSheet(StyleHelper::getHeaderStyle());
	ui->searchLine->setStyleSheet(StyleHelper::getInputLineStyle());
}

BooksOutput::~BooksOutput() noexcept
{
	delete ui;
}