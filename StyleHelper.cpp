#include "StyleHelper.h"

QString StyleHelper::getButtonsStyle() noexcept
{
    return "QPushButton { "
            "   border: 1px solid rgba(100%, 29%, 24%,50%); "
            "   border-radius: 5px; "
            "} "
            "QPushButton::hover { "
            "   background-color: rgba(56%, 55%, 54%, 15%); "
            "} "
            "QPushButton::pressed { "
            "   background-color: rgba(92%, 91%, 86%,100%); "
            "} ";
}

QString StyleHelper::getInputLineStyle() noexcept
{
    return "QLineEdit {"
        "    background-color: rgba(100%, 100%, 100%, 50%);"
        "    border: 1px solid rgba(100%, 100%, 100%, 50%);"
        "    border-radius: 6px; "
        "    color: rgba(76%, 76%, 76%,100%); "
        "}";
}
QString StyleHelper::getChangedInputLineStyle() noexcept
{
    return "QLineEdit {"
        "       background-color: rgba(100%, 100%, 100%, 50%);"
        "       border: 1px solid rgba(100%, 100%, 100%, 50%);"
        "       border-radius: 6px; "
        "       color: rgba(100%, 29%, 24%, 100%); "
        "   }";
}
QString StyleHelper::getMainWidgetStyle() noexcept {
    return  "QWidget#mainWidget {"
        "       border: 1px solid rgba(80%, 23%, 19%, 50%); "
        "       border-radius: 15px; "
        "   }";
}
QString StyleHelper::getCentralWidgetStyle() noexcept {
    return "background-color: rgba(92%, 91%, 86%,100%); "
        "       color: rgba(80%, 23%, 19%, 100%); "
        "       font: \"Cascadia Code light\"; ";
}

QString StyleHelper::getHaveAccountLblStyle() noexcept
{
    return "QLabel::hover { "
        "       color: rgba(80%, 23%, 19%, 75%);"
        "   }"
        "   QLabel::pressed { "
        "       color: rgba(80%, 23%, 19%, 100%);"
        "   }";
}

QString StyleHelper::getHeaderStyle() noexcept
{
    return
        "QWidget#header {"
    "       border-bottom: 1px solid rgba(80%, 23%, 19%, 50%); " 
    "    }";
}

QString StyleHelper::getMainWidgetCheckBookFormStyle() noexcept
{
    return "QWidget#mainWidget{ "
        "      border: 1px solid rgba(80%, 23%, 19%, 50%);"
        "      border-top: none;"
        "   }";
}

QString StyleHelper::getBackBtnStyle() noexcept
{
    return "border: none";
}

QString StyleHelper::getBooksTableStyle() noexcept
{
    return "QWidget#booksTable {"
        "      border-left: 1px solid rgba(80%, 23%, 19%, 50%);"
        "   }";
}

