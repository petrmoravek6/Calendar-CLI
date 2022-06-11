#pragma once
#include <stdexcept>

class FileException : public std::runtime_error {
public:
    FileException(const char * msg)
            : runtime_error(msg) {
    }
};
