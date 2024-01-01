#pragma once
#include "qpushbutton.h"
class ButtonBack : public QPushButton
{
public:
	explicit ButtonBack(QWidget* parent = nullptr) noexcept;
private:
	bool eventFilter(QObject* object, QEvent* event) noexcept override ;
};

