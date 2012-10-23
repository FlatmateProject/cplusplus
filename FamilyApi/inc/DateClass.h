#ifndef DATECLASS_H
#define DATECLASS_H

#include <map>
#include <string>
#include <ctime>

using namespace std;

enum Months{
    noneMonth=-1,
    January=0,
    February,
    March,
    April,
    May,
    June,
    July,
    August,
    September,
    October,
    November,
    December
};

enum Days{
    noneDay=-1,
    Monday=0,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday
};

enum DateCauseCode{
    dateOk  =0,
    badSec  =1,
    badMin  =2,
    badHour =4,
    badDay  =8,
    badMon  =16,
    badYear =32,
    badWday =64,
    badYday =128
};

class DateClass
{
public:
    DateClass();
private:
    int	sec;	/* Seconds: 0-59 (K&R says 0-61?) */
    int	min;	/* Minutes: 0-59 */
    int	hour;	/* Hours since midnight: 0-23 */
    int	mday;	/* Day of the month: 1-31 */
    Months      mon;	/* Months *since* january: 0-11 */
    int         year;
    Days        wday;	/* Days since Sunday (0-6) */
    int         yday;   /* Days since Jan. 1: 0-366 */

    map < int , string > causeExplanation;

    bool validRange(int value, int range);
    void initiateExplanation();
    void initiateAttributes();
    bool isLeapYear(int year);
    int getNumberOfDaysInMonth(Months mon);
    int validDate(int causeCode, string &cause);

    int setSec(int sec);
    int setMin(int min);
    int setHour(int hour);
    int setMday(int day);
    int setMon(Months mon);
    int setYear(int year);
    int setWday();
    int setYday();

 public:
    int setDate(tm date, string &cause);

    int getSec();
    int getMin();
    int getHour();
    int getMday();
    int getMon();
    int getYear();
    int getWday();
    int getYday();
};



#endif // DATECLASS_H
