/********************************************************************************
** Form generated from reading UI file 'MainWindowVbYSrP.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MAINWINDOWVBYSRP_H
#define MAINWINDOWVBYSRP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget* centralwidget;
    QGridLayout* gridLayout;
    QSpacerItem* entryRegSpacer;
    QSpacerItem* logPassSpacer;
    QSpacerItem* generalBottomSpacer;
    QPushButton* entryBtn;
    QSpacerItem* EnrtyRegSpacer;
    QHBoxLayout* logHLyt;
    QSpacerItem* logLeftSpacer;
    QLabel* login;
    QLineEdit* loginInpLine;
    QSpacerItem* logRightSpacer;
    QPushButton* regBtn;
    QSpacerItem* passEntrySpacer;
    QSpacerItem* logTittleSpacer;
    QSpacerItem* generalTopSpacer;
    QLabel* tittle;
    QHBoxLayout* passHLyt;
    QSpacerItem* passLeftSpacer;
    QLabel* password;
    QLineEdit* passInpLine;
    QSpacerItem* passRightSpacer;
    QLabel* hint;

    void setupUi(QMainWindow* MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(650, 431);
        MainWindow->setInputMethodHints(Qt::ImhHiddenText);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setMinimumSize(QSize(650, 300));
        centralwidget->setStyleSheet(QString::fromUtf8(""));
        centralwidget->setInputMethodHints(Qt::ImhHiddenText);
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        entryRegSpacer = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Maximum);

        gridLayout->addItem(entryRegSpacer, 11, 0, 1, 1);

        logPassSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Maximum);

        gridLayout->addItem(logPassSpacer, 5, 0, 1, 1);

        generalBottomSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(generalBottomSpacer, 14, 0, 1, 1);

        entryBtn = new QPushButton(centralwidget);
        entryBtn->setObjectName("entryBtn");
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(entryBtn->sizePolicy().hasHeightForWidth());
        entryBtn->setSizePolicy(sizePolicy);
        entryBtn->setMinimumSize(QSize(250, 35));
        entryBtn->setMaximumSize(QSize(250, 35));
        entryBtn->setBaseSize(QSize(0, 0));
        QFont font;
        font.setFamilies({ QString::fromUtf8("Cascadia Code Light") });
        font.setPointSize(18);
        font.setBold(false);
        font.setItalic(false);
        entryBtn->setFont(font);
        entryBtn->setCursor(QCursor(Qt::PointingHandCursor));
        entryBtn->setFocusPolicy(Qt::StrongFocus);
        entryBtn->setAutoFillBackground(false);
        entryBtn->setStyleSheet(QString::fromUtf8(""));

        gridLayout->addWidget(entryBtn, 9, 0, 1, 1, Qt::AlignHCenter);

        EnrtyRegSpacer = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Maximum);

        gridLayout->addItem(EnrtyRegSpacer, 10, 0, 1, 1);

        logHLyt = new QHBoxLayout();
        logHLyt->setSpacing(10);
        logHLyt->setObjectName("logHLyt");
        logHLyt->setContentsMargins(0, 0, 0, 0);
        logLeftSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        logHLyt->addItem(logLeftSpacer);

        login = new QLabel(centralwidget);
        login->setObjectName("login");
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(login->sizePolicy().hasHeightForWidth());
        login->setSizePolicy(sizePolicy1);
        login->setMinimumSize(QSize(105, 50));
        QFont font1;
        font1.setFamilies({ QString::fromUtf8("Cascadia Code Light") });
        font1.setPointSize(22);
        font1.setBold(false);
        font1.setItalic(false);
        login->setFont(font1);
        login->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignVCenter);

        logHLyt->addWidget(login);

        loginInpLine = new QLineEdit(centralwidget);
        loginInpLine->setObjectName("loginInpLine");
        loginInpLine->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(loginInpLine->sizePolicy().hasHeightForWidth());
        loginInpLine->setSizePolicy(sizePolicy2);
        loginInpLine->setMinimumSize(QSize(350, 45));
        loginInpLine->setMaximumSize(QSize(350, 45));
        QFont font2;
        font2.setFamilies({ QString::fromUtf8("Calibri Light") });
        font2.setPointSize(16);
        font2.setBold(false);
        font2.setItalic(false);
        loginInpLine->setFont(font2);
        loginInpLine->setCursor(QCursor(Qt::IBeamCursor));
        loginInpLine->setStyleSheet(QString::fromUtf8(""));
        loginInpLine->setInputMethodHints(Qt::ImhPreferNumbers);
        loginInpLine->setMaxLength(32);
        loginInpLine->setFrame(false);
        loginInpLine->setCursorPosition(20);

        logHLyt->addWidget(loginInpLine);

        logRightSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        logHLyt->addItem(logRightSpacer);


        gridLayout->addLayout(logHLyt, 4, 0, 1, 1);

        regBtn = new QPushButton(centralwidget);
        regBtn->setObjectName("regBtn");
        sizePolicy.setHeightForWidth(regBtn->sizePolicy().hasHeightForWidth());
        regBtn->setSizePolicy(sizePolicy);
        regBtn->setMinimumSize(QSize(250, 35));
        regBtn->setMaximumSize(QSize(250, 35));
        regBtn->setFont(font);
        regBtn->setCursor(QCursor(Qt::PointingHandCursor));
        regBtn->setContextMenuPolicy(Qt::DefaultContextMenu);
        regBtn->setStyleSheet(QString::fromUtf8(""));

        gridLayout->addWidget(regBtn, 12, 0, 1, 1, Qt::AlignHCenter | Qt::AlignVCenter);

        passEntrySpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Maximum);

        gridLayout->addItem(passEntrySpacer, 8, 0, 1, 1);

        logTittleSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Maximum);

        gridLayout->addItem(logTittleSpacer, 2, 0, 1, 1);

        generalTopSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(generalTopSpacer, 0, 0, 1, 1);

        tittle = new QLabel(centralwidget);
        tittle->setObjectName("tittle");
        QFont font3;
        font3.setFamilies({ QString::fromUtf8("Cascadia Code SemiLight") });
        font3.setPointSize(36);
        font3.setBold(false);
        font3.setItalic(false);
        tittle->setFont(font3);
        tittle->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(tittle, 1, 0, 1, 1);

        passHLyt = new QHBoxLayout();
        passHLyt->setSpacing(10);
        passHLyt->setObjectName("passHLyt");
        passHLyt->setContentsMargins(0, 0, 0, 0);
        passLeftSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        passHLyt->addItem(passLeftSpacer);

        password = new QLabel(centralwidget);
        password->setObjectName("password");
        sizePolicy1.setHeightForWidth(password->sizePolicy().hasHeightForWidth());
        password->setSizePolicy(sizePolicy1);
        password->setMinimumSize(QSize(105, 50));
        password->setFont(font1);
        password->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignVCenter);

        passHLyt->addWidget(password);

        passInpLine = new QLineEdit(centralwidget);
        passInpLine->setObjectName("passInpLine");
        passInpLine->setEnabled(true);
        sizePolicy2.setHeightForWidth(passInpLine->sizePolicy().hasHeightForWidth());
        passInpLine->setSizePolicy(sizePolicy2);
        passInpLine->setMinimumSize(QSize(350, 45));
        passInpLine->setMaximumSize(QSize(350, 45));
        passInpLine->setFont(font2);
        passInpLine->setFocusPolicy(Qt::StrongFocus);
        passInpLine->setStyleSheet(QString::fromUtf8(""));
        passInpLine->setInputMethodHints(Qt::ImhNone);
        passInpLine->setMaxLength(32);
        passInpLine->setFrame(false);
        passInpLine->setCursorPosition(11);
        passInpLine->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignVCenter);

        passHLyt->addWidget(passInpLine);

        passRightSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        passHLyt->addItem(passRightSpacer);


        gridLayout->addLayout(passHLyt, 6, 0, 1, 1);

        hint = new QLabel(centralwidget);
        hint->setObjectName("hint");

        gridLayout->addWidget(hint, 7, 0, 1, 1, Qt::AlignHCenter);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        entryBtn->setDefault(false);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow* MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        centralwidget->setWindowTitle(QCoreApplication::translate("MainWindow", "Form", nullptr));
#if QT_CONFIG(statustip)
        entryBtn->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        entryBtn->setText(QCoreApplication::translate("MainWindow", "\320\222\321\205\320\276\320\264", nullptr));
        login->setText(QCoreApplication::translate("MainWindow", "\320\233\320\276\320\263\320\270\320\275", nullptr));
        loginInpLine->setText(QCoreApplication::translate("MainWindow", "janasman17@gmail.com", nullptr));
        loginInpLine->setPlaceholderText(QString());
        regBtn->setText(QCoreApplication::translate("MainWindow", "\320\240\320\265\320\263\320\270\321\201\321\202\321\200\320\260\321\206\320\270\321\217", nullptr));
        tittle->setText(QCoreApplication::translate("MainWindow", "\320\255\320\273\320\265\320\272\321\202\321\200\320\276\320\275\320\275\320\260\321\217 \320\261\320\270\320\261\320\273\320\270\320\276\321\202\320\265\320\272\320\260", nullptr));
        password->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\321\200\320\276\320\273\321\214", nullptr));
        passInpLine->setText(QCoreApplication::translate("MainWindow", "Q34fs_pfeas", nullptr));
        passInpLine->setPlaceholderText(QString());
        hint->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow : public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MAINWINDOWVBYSRP_H
