#include "calendarView.h"
#include "../time/date.h"
#include "../util/util.h"
#include <iostream>
#include <iomanip>
using namespace std;

CCalendarView::CCalendarView (const CDate & currDay,
                              const std::unordered_map<size_t, CEvent> & mainDb)
                              : m_SelectedTime(currDay), m_MainDatabase(mainDb) {
}

int CCalendarView::menu() const {
    printMenu();
    return getNumber(0, 6);
}

void CCalendarView::printMenu() const {
    cout << endl << "KALENDARNI MENU" << endl;
    cout << "\t0) Zpet do hlavniho menu" << "\t1) Zobrazit predchozi" << "\t2) Zobrazit nasledujici" << endl;
    cout << "\t3) Zobrazit detaily udalosti" << "\t4) Pridat udalost" << "\t5) Upravit udalost" << endl;
    cout << "\t6) Smazat udalost" << endl;
    cout << setw(c_DelimBarWidth) << setfill('=') << "" << endl << endl;
    cout << "Zadejte jednu z moznosti a enterem potvrdte:" << endl;
}

size_t CCalendarView::selectEvent() const {
    cout << "Zadejte index udalosti (cislo zobrazene vedle udalosti)." << endl;
    int idx = getNumber(0, m_EventIdxToId.size() - 1);
    return m_EventIdxToId.at(idx);
}

bool CCalendarView::isEmpty() const {
    return m_EventIdxToId.empty();
}
