#include "AuxilFunctns.h"
#include <codecvt>
#include <locale>

std::string getSharedSalt() {
    return "5n6liVhczEDhnb3twBzbwzj2IACYgiRGi58MAhikZSldtBCRhI4i6pwgq2qmmCHt";
}
/*
sql::ConnectOptionsMap setConnectionOptions() {
    sql::ConnectOptionsMap connection_options{};
    connection_options["hostName"] = "tcp://127.0.0.1:3306";
    connection_options["userName"] = "root";
    connection_options["password"] = "kriogen701";
    connection_options["schema"] = "digLib";
    connection_options["characterSetResults"] = "cp1251";
    connection_options["OPT_CHARSET_NAME"] = "cp1251";
    connection_options["OPT_SET_CHARSET_NAME"] = "cp1251";
    return connection_options;
}
*/