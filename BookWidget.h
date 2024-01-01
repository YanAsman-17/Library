#pragma once
#include <qwidget.h>
#include "BooksOutput.h"

class BookWidget : public QWidget
{
public:
	explicit BookWidget(BooksOutput* booksTable_ = nullptr,QWidget* parent = nullptr ) noexcept;
private:
	BooksOutput* booksTable;

	bool eventFilter(QObject* object, QEvent* event) noexcept;
};


class EmptyBookWidget : public BookWidget
{
public:
	explicit EmptyBookWidget(QWidget* parent = nullptr) noexcept;
private:
	bool eventFilter(QObject* object, QEvent* event) noexcept;
};
