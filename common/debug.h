#ifndef COMMON_DEBUG_H
#define COMMON_DEBUG_H

#include <string>

namespace Common {

    void exitOnError(const std::string &error, const int status = 1);
}

#endif
