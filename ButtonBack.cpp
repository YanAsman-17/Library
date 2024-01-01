#include "ButtonBack.h"
#include "qevent.h"

ButtonBack::ButtonBack(QWidget* parent) noexcept: QPushButton(parent)
{
	QPushButton* button = qobject_cast<QPushButton*>(this);
	button->setIcon(QIcon("resources/images/btnBack.png"));
	this->installEventFilter(this);
}

bool ButtonBack::eventFilter(QObject* object, QEvent* event) noexcept
{
	QPushButton* button = qobject_cast<QPushButton*>(object);
	if (!button) {
		return false;
	}
	if (object == this && event->type() == QEvent::Enter) {
		button->setIcon(QIcon("resources/images/btnBackHover.png"));
		return true;
	}
	if (object == this && event->type() == QEvent::Leave) {
		button->setIcon(QIcon("resources/images/btnBack.png"));
		return true;
	}
	return false;
}
