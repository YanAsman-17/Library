/*
#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include "LoginForm.h"
#include "Navigation.h"

Navigation::Navigation(QWidget* parent) : QMainWindow(parent)
{
    // Создание стека виджетов
    stackedWidget = new QStackedWidget(this);
    // Создание экранов
    LoginForm* login = new LoginForm();
    login->setFixedSize(stackedWidget->size());
    // Добавление экранов в стек виджетов
    stackedWidget->addWidget(login);

    // Переключение на экран входа
    stackedWidget->setCurrentIndex(0);

    //QRect availableGeometry = QDesktopWidget().availableGeometry(this);
    
    // Взаимодействие с элементами управления на экране входа
    connect(login->ui->regBtn, &QPushButton::clicked, [=]() {
        // Проверка логина и пароля
        // Если успешно, переключение на экран меню
        stackedWidget->setCurrentIndex(1);
        });
        
    setCentralWidget(stackedWidget);
}

void Navigation::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    for (int i = 0; i < stackedWidget->count(); i++) {
        QWidget* widget = stackedWidget->widget(i);
        widget->setFixedSize(stackedWidget->size());
    }
}
*/