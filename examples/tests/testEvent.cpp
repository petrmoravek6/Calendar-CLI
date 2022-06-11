#include "../../src/time/date.h"
#include "../../src/time/clock.h"
#include "../../src/event/event.h"
#include <cassert>

using namespace std;

int main () {
    CEvent e1("Name1", STimePeriod(CDate(2022, 5, 15), CClock(20, 0), CDate(2022, 5, 15), CClock(23, 21)),
              nullopt, set<string>(), nullopt, (optional<std::unique_ptr<CRepeat>> &) nullopt, true);
    CEvent e2("Name2", STimePeriod(CDate(2022, 5, 15), CClock(20, 0), CDate(2022, 5, 18), CClock(15, 0)),
              nullopt, set<string>(), nullopt, (optional<std::unique_ptr<CRepeat>> &) nullopt, true);
    CEvent e3("Name3", STimePeriod(CDate(2022, 6, 6), CClock(23, 30), CDate(2022, 6, 7), CClock(6, 0)),
              nullopt, set<string>{"Pepa", "Jirka"}, {"detail"}, (optional<std::unique_ptr<CRepeat>> &) nullopt, false);

    set<SEventDayLayout> lay1 = e1.generateDayLayouts(0);
    set<SEventDayLayout> lay2 = e2.generateDayLayouts(1);
    set<SEventDayLayout> lay3 = e3.generateDayLayouts(2);

    assert(lay1.size() == 1);
    assert(lay1.begin()->hasSameParameters(SEventDayLayout(CDate(2022, 5, 15), CClock(20, 0), CClock(23, 21), 0)));
    assert(lay2.size() == 4);
    assert(lay2.begin()->hasSameParameters(SEventDayLayout(CDate(2022, 5, 15), CClock(20, 0), CClock(23, 59), 1)));
    assert(next(lay2.begin())->hasSameParameters(SEventDayLayout(CDate(2022, 5, 16), CClock(0, 0), CClock(23, 59), 1)));
    assert(next(next(lay2.begin()))->hasSameParameters(SEventDayLayout(CDate(2022, 5, 17), CClock(0, 0), CClock(23, 59), 1)));
    assert(next(next(next(lay2.begin())))->hasSameParameters(SEventDayLayout(CDate(2022, 5, 18), CClock(0, 0), CClock(15, 00), 1)));
    assert(lay3.size() == 2);
    assert(lay3.begin()->hasSameParameters(SEventDayLayout(CDate(2022, 6, 6), CClock(23, 30), CClock(23, 59), 2)));
    assert(next(lay3.begin())->hasSameParameters(SEventDayLayout(CDate(2022, 6, 7), CClock(0, 0), CClock(6, 0), 2)));

    //------------------------------------------------------------------------------------------------------------------

    assert(e1.takesPlaceAfter({CDate(2010, 5 , 5), CClock(10, 00)}));
    assert(e1.takesPlaceAfter({CDate(2022, 5 , 15), CClock(23, 20)}));
    assert(!e1.takesPlaceAfter({CDate(2022, 5 , 15), CClock(23, 21)}));
    assert(!e1.takesPlaceAfter({CDate(2022, 5 , 15), CClock(23, 22)}));
    assert(e2.takesPlaceAfter({CDate(2022, 5 , 16), CClock(10, 00)}));
    assert(e2.takesPlaceAfter({CDate(2022, 5 , 17), CClock(16, 00)}));
    assert(!e2.takesPlaceAfter({CDate(2022, 5 , 18), CClock(16, 00)}));

}