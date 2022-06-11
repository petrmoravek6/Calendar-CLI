#include "util.h"
#include "../time/date.h"
#include "../time/clock.h"
#include <iostream>
#include <chrono>
#include <thread>
using namespace std;

int getNumber(long min, long max) {
    int choice;
    while (true) {
        while (!(cin >> choice)) {
            if (cin.eof()) {
                cout << endl << "Doslo k necekanemu vstupu uvnitr programu (EOF - end of file). Program bude nasilim ukoncen." << endl;
                exit(1);
            }
            cout << "Chybne zadany vstup, opakujte akci" << endl;
            cin.clear();
            cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
        if (choice < min || choice > max) {
            cout << "Bylo zadano cislo mimo rozmezi, opakujte akci." << endl;
        }
        else {
            return choice;
        }
    }
}

string getLine(long maxLength) {
    unique_ptr<char[]> input(new char[maxLength + 1]);
    while (true) {
        cin.getline(input.get(), maxLength + 1);
        if (cin.eof()) {
            cout << endl << "Doslo k necekanemu vstupu uvnitr programu (EOF - end of file). Program bude nasilim ukoncen." << endl;
            exit(1);
        }
        if (cin.good()) {
            break;
        }
        cout << "Chyba, zadej znovu (zkontroluj maximalni delku: " << maxLength << " znaku)" << endl;
        cin.clear();
        cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
    }
    string res(input.get());
    return res;
}

STimeStamp getTime() {
    char del1, del2, del3;
    int day, month, year, hour, min;
    while (true) {
        if (scanf("%d%c%d%c%d %d%c%d", &day, &del1, &month, &del2, &year, &hour, &del3, &min) != 8
            || del1 != '/' || del2 != '/' || del3 != ':') {
            if (cin.eof()) {
                cout << endl << "Doslo k necekanemu vstupu uvnitr programu (EOF - end of file). Program bude nasilim ukoncen." << endl;
                exit(1);
            }
            cout << "Spatne zadany cas, opakujte akci" << endl;
            cin.clear();
            cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        try {
            CDate date(year, month, day);
            CClock clock(hour, min);
            cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
            return {date, clock};
        } catch (const InvalidDateException & e1) {
            cout << "Nespravne zadany datum (zkontrolujte rok v rozmezi 1970 - 2037), opakujte akci" << endl;
            cin.clear();
            cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
        } catch (const InvalidClockException & e2) {
            cout << "Nespravne zadany cas, opakujte akci" << endl;
            cin.clear();
            cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

void printMsgAndSleep(const string & msg, int64_t sec) {
    cout << msg << endl;
    this_thread::sleep_for(chrono::seconds(sec));
}