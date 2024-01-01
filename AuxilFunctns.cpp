#include "AuxilFunctns.h"
#include <codecvt>
#include <locale>

std::string getSharedSalt() {
    return "5n6liVhczEDhnb3twBzbwzj2IACYgiRGi58MAhikZSldtBCRhI4i6pwgq2qmmCHt";
}
std::string getSalt() {
    char symb[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    srand(time(NULL));
    const int saltSize = 64;
    std::string salt;
    salt.resize(saltSize);
    for (int i = 0; i < saltSize; ++i) {
        salt[i] = symb[rand() % (sizeof(symb) / sizeof(char) - 1)];
    }
    return salt;
}
