#pragma once
#include "qmainwindow.h"
#include "x64/Release/uic/ui_AdminMenuForm.h"

class AdminMenuForm : public QMainWindow
{
public:
	explicit AdminMenuForm(int userId_ = 0,QWidget* parent = nullptr) noexcept;
	~AdminMenuForm() noexcept;
	friend class Navigation;
private:
	Ui::AdminMenuForm* ui;
	int userId;

	void setInterfaceStyle() noexcept;
};

