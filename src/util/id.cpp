#include "id.h"

size_t ID::nextId() {
    return c_Id++;
}

size_t ID::c_Id = 0;