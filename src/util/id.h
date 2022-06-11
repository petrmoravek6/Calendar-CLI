#pragma once
#include <cstddef>

class ID {
public:
    /**
     * @return Unique ID
     */
    static size_t nextId();
private:
    static size_t c_Id;
};


