#include "BookWidget.h"
#include "qevent.h"
#include <iostream>


// BookWidget
BookWidget::BookWidget(BooksOutput* booksTable_, QWidget* parent) noexcept : booksTable(booksTable_)
{
    this->installEventFilter(this);
}

bool BookWidget::eventFilter(QObject* object, QEvent* event) noexcept
{
    if (object == this && event->type() == QEvent::MouseButtonPress) {
        emit booksTable->checkBook(booksTable->getUserId(), this->property("id").toInt());
    }
    if (object == this && event->type() == QEvent::Enter) {
        this->setStyleSheet("background-color: rgba(56%, 55%, 54%, 100%); color: black;");
    }
    else if (object == this &&  event->type() == QEvent::Leave)
    {
        this->setStyleSheet("");
    }
    return false;
}

// EmptyBookWidget

EmptyBookWidget::EmptyBookWidget(QWidget* parent) noexcept : BookWidget(nullptr,parent)
{

}

bool EmptyBookWidget::eventFilter(QObject* object, QEvent* event) noexcept // переопределил eventFilter базового класса 
{
    return false;
}
