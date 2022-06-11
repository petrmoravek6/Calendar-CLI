#include "../../src/time/date.h"
#include "../../src/time/clock.h"
#include <cassert>
using namespace std;

/** Same function as getTime() in src, except it's adjusted for testing **/
bool getTimeTester(const char * stream) {
    char del1, del2, del3;
    int day, month, year, hour, min;
    if (sscanf(stream, "%d%c%d%c%d %d%c%d", &day, &del1, &month, &del2, &year, &hour, &del3, &min) != 8
        || del1 != '/' || del2 != '/' || del3 != ':') {
        return false;
    }
    try {
        CDate date(year, month, day);
        CClock clock(hour, min);
        return true;
    } catch (const InvalidDateException & e1) {
        return false;
    } catch (const InvalidClockException & e2) {
        return false;
    }
}


int main () {
    assert(!getTimeTester("blabla"));
    assert(!getTimeTester("1551"));
    assert(!getTimeTester(""));
    assert(!getTimeTester("\n"));
    assert(!getTimeTester("   "));
    assert(!getTimeTester("\t"));
    assert(!getTimeTester("12.12.2010 8:57"));
    assert(!getTimeTester("12/12.2010 8:57"));
    assert(!getTimeTester("12.12/2010 8:57"));
    assert(!getTimeTester("12/12/2010 8.57"));
    assert(!getTimeTester("12/12/ 8:57"));
    assert(!getTimeTester("/12/2010 8:57"));
    assert(!getTimeTester("12/2010 8:57"));
    assert(!getTimeTester("12/12/2010 8:"));
    assert(!getTimeTester("12/12/2010 8"));
    assert(!getTimeTester("12/12/2010"));
    assert(!getTimeTester("8:57"));
    assert(!getTimeTester("X/12/2010 8:57"));
    assert(!getTimeTester("12/X/2010 8:57"));
    assert(!getTimeTester("12/12/X 8:57"));
    assert(!getTimeTester("12/12/2010 X:57"));
    assert(!getTimeTester("12/12/2010 8:X"));
    assert(!getTimeTester("-1/12/2010 8:57"));
    assert(!getTimeTester("0/12/2010 8:57"));
    assert(!getTimeTester("40/12/2010 8:57"));
    assert(!getTimeTester("1/-1/2010 8:57"));
    assert(!getTimeTester("1/0/2010 8:57"));
    assert(!getTimeTester("1/420/2010 8:57"));
    assert(!getTimeTester("1/12/0 8:57"));
    assert(!getTimeTester("1/12/-1 8:57"));
    assert(!getTimeTester("1/12/1969 8:57"));
    assert(!getTimeTester("1/12/2039 8:57"));
    assert(!getTimeTester("1/12/200 8:57"));
    assert(!getTimeTester("29/2/2022 8:57"));
    assert(!getTimeTester("31/4/2022 8:57"));
    assert(!getTimeTester("31/4/2023 8:57"));
    assert(!getTimeTester("31/4/2024 8:57"));
    assert(!getTimeTester("3/4/2024 05:60"));
    assert(!getTimeTester("3/4/2024 8:61"));
    assert(!getTimeTester("3/4/2024 8:-2"));
    assert(!getTimeTester("3/4/2024 -1:57"));
    assert(!getTimeTester("3/4/2024 24:57"));
    assert(!getTimeTester("3/4/2024 42:57"));
    assert(getTimeTester("12/12/2010 8:57"));
    assert(getTimeTester("01/01/2010 00:00"));
    assert(getTimeTester("12/12/2010 01:57"));
    assert(getTimeTester("29/2/2024 8:57"));
    assert(getTimeTester("31/01/2022 06:15"));
    assert(getTimeTester("3/4/2024 0:0"));

}