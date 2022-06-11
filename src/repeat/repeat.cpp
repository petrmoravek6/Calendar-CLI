#include "repeat.h"
#include <iostream>
using namespace std;

ostream& operator << (ostream &os, const CRepeat & repeat) {
    repeat.print(os);
    return os;
}
