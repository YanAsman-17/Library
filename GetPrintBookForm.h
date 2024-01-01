#pragma once
#include <qmainwindow.h>
#include "x64/Release/uic/ui_GetPrintBookForm.h"

enum class PrintBookFormIndexes {
	MAIN_FORM,
	RETURN_FORM,
	SUCCESS_FORM,
	ERROR_FORM
};

class GetPrintBookForm : public QMainWindow
{
public:
	explicit GetPrintBookForm(QWidget* parent = nullptr) noexcept;
	~GetPrintBookForm() noexcept;
	friend class CheckBookForm;
private:
	Ui::GetPrintBookForm* ui;
	void setInterfaceStyle() noexcept;
};

