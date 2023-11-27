#include "StyleHelper.h"

QString StyleHelper::getButtonsStyle()
{
    return "QPushButton { "
            "   border: 1px solid rgba(100%, 29%, 24%,50%); "
            "   border-radius: 3px; "
            "} "
            "QPushButton::hover { "
            "   background-color: rgba(56%, 55%, 54%, 15%); "
            "} "
            "QPushButton::pressed { "
            "   background-color: rgba(92%, 91%, 86%,100%); "
            "} ";
}

QString StyleHelper::getInputLineStyle()
{
    return "QLineEdit {"
        "    background-color: rgba(100%, 100%, 100%, 50%);"
        "    border: 1px solid rgba(100%, 100%, 100%, 40%);"
        "    border-radius: 5px; "
        "    color: rgba(66%, 90%, 81%, 90%); "
        "}";
}
QString StyleHelper::getChangedInputLineStyle()
{
    return "QLineEdit {"
        "       background-color: rgba(100%, 100%, 100%, 50%);"
        "       border: 1px solid rgba(100%, 100%, 100%, 40%);"
        "       border-radius: 5px; "
        "       color: rgba(100%, 29%, 24%, 100%); "
        "   }";
}
QString StyleHelper::getWidgetStyle() {
    return "background-color: rgba(92%, 91%, 86%,100%); "
           "color: rgba(100%, 29%, 24%, 100%); "
           "font: \"Cascadia Code light\"; ";
}
