#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h> 
#include <cppconn/prepared_statement.h>
#include "Navigation.h"



Navigation::Navigation(QWidget* parent) noexcept
    : QMainWindow(parent)
{
    
    try {
        sql::Driver* driver = get_driver_instance();
        conn.reset(driver->connect("tcp://127.0.0.1:3306", "root", "password"));
        conn->setSchema("databaseName");
    }
    catch (const sql::SQLException& ex) {
        std::cerr << "problem with connection in Navigation::Navigation" << std::endl;
    }

    stackedWidget = std::make_unique<QStackedWidget>(this);

    logIn = std::make_unique<LoginForm>(conn);
    registr = std::make_unique<RegistrForm>(conn);
    menu = std::make_unique<MenuForm>();
    libBooks = std::make_unique<LibBooksForm>(conn);  
    checkBook = std::make_unique<CheckBookForm>(conn);
    userBooks = std::make_unique<UserBooksForm>(conn);
    bookReader = std::make_unique<BookReaderForm>();
    adminMenu = std::make_unique<AdminMenuForm>();
    adminMain = std::make_unique<AdminMainForm>(conn);
    
    stackedWidget->addWidget(logIn.get()); 
    stackedWidget->addWidget(registr.get());
    stackedWidget->addWidget(menu.get());
    stackedWidget->addWidget(libBooks.get());
    stackedWidget->addWidget(checkBook.get());
    stackedWidget->addWidget(userBooks.get());
    stackedWidget->addWidget(bookReader.get());
    stackedWidget->addWidget(adminMenu.get());
    stackedWidget->addWidget(adminMain.get());
    
    stackedWidget->setCurrentIndex(static_cast<int>(NavigationIndexes::LOG_IN));
    logIn->resetForm();

    setWindowTitle(logIn->windowTitle());
    QIcon iconApp("resources/images/iconApp.png");
    setWindowIcon(iconApp);
    
    
    // кнопки формы входа
    connect(logIn->ui->regBtn, &QPushButton::clicked,this ,[=]() {
        registr->resetForm();
        stackedWidget->setCurrentIndex(static_cast<int>(NavigationIndexes::REGISTER));
        setWindowTitle(registr->windowTitle());
        });

    connect(logIn->ui->entryBtn, &QPushButton::clicked, this, [=]() {
        UserInfrom userInform = logIn->checkUser();
        if (userInform.userId != 0 && !userInform.isAdmin ) {
            menu->userId = userInform.userId;
            stackedWidget->setCurrentIndex(static_cast<int>(NavigationIndexes::MENU));
            setWindowTitle(menu->windowTitle());
        }
        else if (userInform.userId != 0 && userInform.isAdmin ) {
            adminMenu->userId = userInform.userId;
            stackedWidget->setCurrentIndex(static_cast<int>(NavigationIndexes::ADMIN_MENU));
            setWindowTitle(adminMenu->windowTitle());
        }
        });

    // кнопки формы регистрации
    registr->ui->lblHaveAccount->installEventFilter(this);

    connect(registr->ui->regBtn, &QPushButton::clicked,this, [=]() {
        int userId = registr->addUser();
        if (userId) {
            registr->resetForm();
            menu->userId = userId;
            stackedWidget->setCurrentIndex(static_cast<int>(NavigationIndexes::MENU));
            setWindowTitle(menu->windowTitle());
        }
        });
    //кнопки меню администратора
    connect(adminMenu->ui->logOutBtn, &QPushButton::clicked, this, [=]() {
        logIn->resetForm();
        stackedWidget->setCurrentIndex(static_cast<int>(NavigationIndexes::LOG_IN));
        setWindowTitle(logIn->windowTitle());
        });
    connect(adminMenu->ui->mainAdmBtn, &QPushButton::clicked, this, [=]() {
        adminMain->resetForm();
        stackedWidget->setCurrentIndex(static_cast<int>(NavigationIndexes::ADMIN_MAIN));
        setWindowTitle(adminMain->windowTitle());
        });
    //кнопки основного окна администратора
    connect(adminMain->ui->backBtn, &QPushButton::clicked, this, [=]() {
        stackedWidget->setCurrentIndex(static_cast<int>(NavigationIndexes::ADMIN_MENU));
        setWindowTitle(adminMenu->windowTitle());
    });


    //кнопки меню
    connect(menu->ui->logOutBtn, &QPushButton::clicked, this, [=]() {
        logIn->resetForm();
        stackedWidget->setCurrentIndex(static_cast<int>(NavigationIndexes::LOG_IN));
        setWindowTitle(logIn->windowTitle());
        });

    connect(menu->ui->libraryBtn, &QPushButton::clicked, this, [=]() {
        libBooks->userId = menu->userId;
        libBooks->resetForm();
        stackedWidget->setCurrentIndex(static_cast<int>(NavigationIndexes::LIB_BOOKS));
        setWindowTitle(libBooks->windowTitle());
        });

    connect(menu->ui->userBooksBtn, &QPushButton::clicked, this, [=]() {
        userBooks->userId = menu->userId;
        userBooks->resetForm();
        stackedWidget->setCurrentIndex(static_cast<int>(NavigationIndexes::USER_BOOKS));
        setWindowTitle(userBooks->windowTitle());
        });

    // кнопки каталога книг библиотеки
    connect(libBooks->ui->backBtn, &QPushButton::clicked, this, [=]() {
        menu->userId = libBooks->userId;
        stackedWidget->setCurrentIndex(static_cast<int>(NavigationIndexes::MENU));
        setWindowTitle(menu->windowTitle());
        });
    connect(libBooks.get(), &LibBooksForm::checkBook, this, [=](int userId, int book_id) {
        checkBook->resetForm(NavigationIndexes::LIB_BOOKS,userId,book_id);
        stackedWidget->setCurrentIndex(static_cast<int>(NavigationIndexes::CHECK_BOOK));
        setWindowTitle(checkBook->windowTitle());
        });

    // кнопки каталога книг пользователя
    connect(userBooks->ui->backBtn, &QPushButton::clicked, this, [=]() {
        menu->userId = userBooks->userId; 
        stackedWidget->setCurrentIndex(static_cast<int>(NavigationIndexes::MENU));
        setWindowTitle(menu->windowTitle());
        });

    connect(userBooks.get(), &UserBooksForm::checkBook, this, [=](int userId, int book_id) {
        checkBook->resetForm(NavigationIndexes::USER_BOOKS,userId, book_id);
        stackedWidget->setCurrentIndex(static_cast<int>(NavigationIndexes::CHECK_BOOK));
        setWindowTitle(checkBook->windowTitle());
        });

    // кнопки просмотра книги
    connect(checkBook->ui->backBtn, &QPushButton::clicked, this, [=]() {
        if (checkBook->prevForm == NavigationIndexes::LIB_BOOKS) {
            stackedWidget->setCurrentIndex(static_cast<int>(NavigationIndexes::LIB_BOOKS));
            setWindowTitle(libBooks->windowTitle());
        }
        else if (checkBook->prevForm == NavigationIndexes::USER_BOOKS) {
            userBooks->booksOutput();
            stackedWidget->setCurrentIndex(static_cast<int>(NavigationIndexes::USER_BOOKS));
            setWindowTitle(userBooks->windowTitle());
        }
        });
    connect(checkBook.get(), &CheckBookForm::openEBookSignal, this, [=](QString bookPath, QString bookName) {
        bookReader->resetForm(bookPath, bookName);
        stackedWidget->setCurrentIndex(static_cast<int>(NavigationIndexes::BOOK_READER));
        setWindowTitle(bookReader->windowTitle());
        });
    //кнопки книги
    connect(bookReader->ui->backBtn, &QPushButton::clicked, this, [=]() {
        stackedWidget->setCurrentIndex(static_cast<int>(NavigationIndexes::CHECK_BOOK));
        setWindowTitle(checkBook->windowTitle());
        });

        
    setCentralWidget(stackedWidget.get());
}
bool Navigation::eventFilter(QObject* object, QEvent* event) noexcept {
    
    if (object == registr->ui->lblHaveAccount && event->type() == QEvent::MouseButtonPress) {
        logIn->resetForm();
        stackedWidget->setCurrentIndex(static_cast<int>(NavigationIndexes::LOG_IN));
        return true;
    }
    
    return false;

}
