#include <iostream>
#include "debug.h"


namespace Common {

    void exitOnError(const std::string &error, const int status)
    {
        if (!error.empty()) {
            std::cerr << error << std::endl;
            exit(status);
        }
    }
}
