#pragma once
#include <qmainwindow.h>
#include "x64/Release/uic/ui_BookReaderForm.h"
#include "NavigationIndexes.h"

class BookReaderForm : public QMainWindow
{
public:
	explicit BookReaderForm(QMainWindow* parent = nullptr) noexcept;
	~BookReaderForm() noexcept;
	friend class Navigation;
private:
	Ui::BookReaderForm* ui;

	void resetForm(QString bookPath, QString bookName) noexcept;
};