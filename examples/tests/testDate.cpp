#include "../../src/time/date.h"
#include <cassert>
#include <ctime>
#include <sstream>

using namespace std;

int main () {
    try {
        time_t t = time(0);   // get time now
        tm * t_tm = localtime(&t);
        CDate d1(*t_tm);
    } catch (const InvalidDateException & e) {
        assert(false);
    }
    try {
        CDate d2(2022, 5, 15);
        CDate d3(2022, 5, 31);
        CDate d4(2024, 2, 29);
        CDate d5(2022, 1, 1);
    } catch (const InvalidDateException & e) {
        assert(false);
    }
    try {
        CDate d6(1800, 5, 5);
        assert(false);

    } catch (const InvalidDateException & e) {
    }
    try {
        CDate d7(2039, 5, 5);
        assert(false);

    } catch (const InvalidDateException & e) {
    }
    try {
        CDate d8(2022, 2, 29);
        assert(false);
    } catch (const InvalidDateException & e) {
    }
    try {
        CDate d9(2010, 4, 31);
        assert(false);
    } catch (const InvalidDateException & e) {
    }
    try {
        CDate d10(2030, -2, 5);
        assert(false);
    } catch (const InvalidDateException & e) {
    }
    try {
        CDate d11(2030, 0, 5);
        assert(false);
    } catch (const InvalidDateException & e) {
    }
    try {
        CDate d12(2030, 13, 5);
        assert(false);
    } catch (const InvalidDateException & e) {
    }
    try {
        CDate d13(2030, 1, 32);
        assert(false);
    } catch (const InvalidDateException & e) {
    }
    try {
        CDate d14(2030, 2, 0);
        assert(false);

    } catch (const InvalidDateException & e) {
    }
    try {
        CDate d15(2030, 4, -1);
        assert(false);

    } catch (const InvalidDateException & e) {
    }

    //------------------------------------------------------------
    CDate d1(2022, 5, 15);
    CDate d2(2022, 5, 16);
    CDate d3(2022, 5, 14);
    CDate d4(2022, 2, 28);
    CDate d5(2022, 2, 1);
    CDate d6(2022, 3, 1);
    CDate d7(2022, 3, 1);
    CDate d8(2023, 3, 1);
    CDate d9(2024, 3, 1);
    assert(d1.daysBetween(d2) == 1);
    assert(d1.daysBetween(d3) == 1);
    assert(d4.daysBetween(d5) == 27);
    assert(d5.daysBetween(d4) == 27);
    assert(d7.daysBetween(d6) == 0);
    assert(d5.daysBetween(d6) == 28);
    assert(d7.daysBetween(d8) == 365);
    assert(d9.daysBetween(d8) == 366);
    //--------------------------------------------------------------
    assert(d9.getMonthName() == "Brezen"s);
    assert(d1.getWeekdayName() == "Nedele"s);
    assert(d1.getWeekdayNameShort() == "NE"s);
    //--------------------------------------------------------------
    assert(d7 == d6);
    assert(d1 < d2);
    assert(d3 < d1);
    assert(d5 < d6);
    assert(d5 != d6);
    assert(d3 < d1);
    assert(d3 != d1);
    //--------------------------------------------------------------
    assert(d9.monthEqual(d8));
    assert(d9.monthEqual(d6));
    assert(d6.monthEqual(d9));
    assert(d1.monthEqual(d2));
    assert(!d5.monthEqual(d6));
    assert(!d6.monthEqual(d5));
    //--------------------------------------------------------------
    ostringstream oss;

    CDate a(2000, 1, 2);
    CDate b(2022, 12, 31);
    oss.str("");
    oss << a;
    assert (oss.str() == "2.1.2000");
    oss . str("");
    oss << b;
    assert (oss.str() == "31.12.2022");
    //--------------------------------------------------------------
    assert(d1.getDateShiftedByDays(1) == d2);
    assert(d1.getDateShiftedByDays(-1) == d3);

    CDate c(2022, 3, 31);
    oss.str("");
    oss << c.getDateShiftedByDays(1);
    assert(oss.str() == "1.4.2022");
    oss.str("");
    oss << c.getDateShiftedByDays(1).getDateShiftedByDays(1);
    assert(oss.str() == "2.4.2022");
    oss.str("");
    oss << c.getDateShiftedByDays(2);
    assert(oss.str() == "2.4.2022");
    oss.str("");
    oss << c.getDateShiftedByDays(3);
    assert(oss.str() == "3.4.2022");
    oss.str("");
    oss << c.getDateShiftedByDays(-1);
    assert(oss.str() == "30.3.2022");
    oss.str("");
    oss << c.getDateShiftedByDays(-1).getDateShiftedByDays(-1);
    assert(oss.str() == "29.3.2022");
    oss.str("");
    oss << c.getDateShiftedByDays(-2);
    assert(oss.str() == "29.3.2022");
    oss.str("");
    oss << c.getDateShiftedByDays(365);
    assert(oss.str() == "31.3.2023");
    oss.str("");
    oss << c.getDateShiftedByDays(365 + 366);
    assert(oss.str() == "31.3.2024");
    CDate d(2022, 1, 31);
    CDate e(2024, 3, 31);
    oss.str("");
    oss << e.getDateShiftedByOneMonth(-1);
    assert(oss.str() == "29.2.2024");
    oss.str("");
    oss << e.getDateShiftedByOneMonth(-1).getDateShiftedByOneMonth(-1);
    assert(oss.str() == "29.1.2024");
    oss.str("");
    oss << e.getDateShiftedByOneMonth(-1).getDateShiftedByOneMonth(-1).getDateShiftedByOneMonth(-1);
    assert(oss.str() == "29.12.2023");
    oss.str("");
    oss << e.getDateShiftedByOneMonth(-1).getDateShiftedByOneMonth(-1).getDateShiftedByOneMonth(-1)
            .getDateShiftedByOneMonth(-1).getDateShiftedByOneMonth(-1).getDateShiftedByOneMonth(-1)
            .getDateShiftedByOneMonth(-1).getDateShiftedByOneMonth(-1).getDateShiftedByOneMonth(-1)
            .getDateShiftedByOneMonth(-1).getDateShiftedByOneMonth(-1).getDateShiftedByOneMonth(-1);
    assert(oss.str() == "29.3.2023");
    oss.str("");
    oss << e.getDateShiftedByOneMonth(-1).getDateShiftedByOneMonth(-1).getDateShiftedByOneMonth(-1)
            .getDateShiftedByOneMonth(-1).getDateShiftedByOneMonth(-1).getDateShiftedByOneMonth(-1)
            .getDateShiftedByOneMonth(-1).getDateShiftedByOneMonth(-1).getDateShiftedByOneMonth(-1)
            .getDateShiftedByOneMonth(-1).getDateShiftedByOneMonth(-1).getDateShiftedByOneMonth(-1)
            .getDateShiftedByOneMonth(-1);
    assert(oss.str() == "28.2.2023");
    //--------------------------------------------------------------
    oss.str("");
    oss << d.getDateShiftedByOneMonth(1);
    assert(oss.str() == "28.2.2022");
    oss.str("");
    oss << d.getDateShiftedByOneMonth(1).getDateShiftedByOneMonth(1);
    assert(oss.str() == "28.3.2022");
    oss.str("");
    oss << d.getDateShiftedByOneMonth(1).getDateShiftedByOneMonth(1).getDateShiftedByOneMonth(1);
    assert(oss.str() == "28.4.2022");
    oss.str("");
    oss << d.getDateShiftedByOneMonth(1).getDateShiftedByOneMonth(1).getDateShiftedByOneMonth(1)
            .getDateShiftedByOneMonth(1).getDateShiftedByOneMonth(1).getDateShiftedByOneMonth(1)
            .getDateShiftedByOneMonth(1).getDateShiftedByOneMonth(1).getDateShiftedByOneMonth(1)
            .getDateShiftedByOneMonth(1).getDateShiftedByOneMonth(1).getDateShiftedByOneMonth(1);
    assert(oss.str() == "28.1.2023");
    //--------------------------------------------------------------
    assert(d1.getFirstDayOfCurrentWeek() == CDate(2022, 5, 9));
    assert(d3.getFirstDayOfCurrentWeek() == CDate(2022, 5, 9));
    assert(d1.getLastDayOfCurrentWeek() == d1);
    assert(d3.getLastDayOfCurrentWeek() == d1);
    assert(d2.getFirstDayOfCurrentWeek() == d2);
    assert(d2.getLastDayOfCurrentWeek() == CDate(2022, 5, 22));
    //--------------------------------------------------------------
    assert(d1.getFirstDayOfCurrentMonth() == CDate(2022, 5, 1));
    assert(d2.getFirstDayOfCurrentMonth() == CDate(2022, 5, 1));
    assert(d3.getFirstDayOfCurrentMonth() == CDate(2022, 5, 1));
    assert(d1.getLastDayOfCurrentMonth() == CDate(2022, 5, 31));
    assert(d2.getLastDayOfCurrentMonth() == CDate(2022, 5, 31));
    assert(d3.getLastDayOfCurrentMonth() == CDate(2022, 5, 31));
    CDate d10(2022, 2, 2);
    CDate d11(2024, 2, 2);
    assert(d10.getFirstDayOfCurrentMonth() == CDate(2022, 2, 1));
    assert(d11.getFirstDayOfCurrentMonth() == CDate(2024, 2, 1));
    assert(d10.getLastDayOfCurrentMonth() == CDate(2022, 2, 28));
    assert(d11.getLastDayOfCurrentMonth() == CDate(2024, 2, 29));
    //--------------------------------------------------------------

}
