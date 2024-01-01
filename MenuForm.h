#pragma once
#include "x64/Release/uic/ui_MenuForm.h"
#include "qwidget.h"

class MenuForm : public QMainWindow {
public:
	explicit MenuForm(int userId_ = 0, QMainWindow* parent = nullptr) noexcept;
	~MenuForm() noexcept;

	friend class Navigation;
private:
	Ui::MenuForm* ui;
	int userId;

	void setInterfaceStyle() noexcept;
};

