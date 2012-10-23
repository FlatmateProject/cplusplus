#include "DateClass.h"

DateClass::DateClass()
{
    initiateExplanation();
    initiateAttributes();
}

bool DateClass::validRange(int value, int range){
    if ((value < 0)||(value >= range)){
        return false;
    }else{
        return true;
    }
}

void DateClass::initiateExplanation(){
    causeExplanation.insert(pair<const int,string>(static_cast<const int>(dateOk),"Every thing looks OK ;)"));
    causeExplanation.insert(pair<const int,string>(static_cast<const int>(badSec),"Given second value is out of range 0-59"));
    causeExplanation.insert(pair<const int,string>(static_cast<const int>(badMin),"Given minut value is out of range 0-59"));
    causeExplanation.insert(pair<const int,string>(static_cast<const int>(badHour),"Given hour value is out of range 0-23"));
    causeExplanation.insert(pair<const int,string>(static_cast<const int>(badDay),"Given day value is out of range of given month"));
    causeExplanation.insert(pair<const int,string>(static_cast<const int>(badMon),"Given month value is out of range 0-11"));
    causeExplanation.insert(pair<const int,string>(static_cast<const int>(badYear),"Given year value is wrong"));
    causeExplanation.insert(pair<const int,string>(static_cast<const int>(badWday),"There is problem to count week day"));
    causeExplanation.insert(pair<const int,string>(static_cast<const int>(badYday),"There is problem to count year day"));
}

void DateClass::initiateAttributes(){
    sec=min=hour=mday=-1;
    mon = noneMonth;
    year=-1;
    wday= noneDay;
    yday=-1;
}

bool DateClass::isLeapYear(int year){
    return ((year%4 == 0 && year%100 != 0) || year%400 == 0);
}

int DateClass::getNumberOfDaysInMonth(Months mon){
    if ((mon == February)&&(isLeapYear(this->year))){
        return 29;
    }else if (mon == February){
        return 28;
    }

    if (mon < 6){
        if (mon%2 == 0){
            return 31;
        }else{
            return 30;
        }
    }else{
        int tmp =mon-6;
        if (tmp%2 == 0){
            return 31;
        }else{
            return 30;
        }
    }
}

int DateClass::validDate(int causeCode, string &cause){
    if (causeCode == dateOk){
        cause += this->causeExplanation.at(static_cast<const int>(dateOk));
        return 1;
    }else if (causeCode & badSec){
        cause += this->causeExplanation.at(static_cast<const int>(badSec))  + '\n';
    }else if (causeCode & badMin){
        cause += this->causeExplanation.at(static_cast<const int>(badMin))  + '\n';
    }else if (causeCode & badHour){
        cause += this->causeExplanation.at(static_cast<const int>(badHour)) + '\n';
    }else if (causeCode & badDay){
        cause += this->causeExplanation.at(static_cast<const int>(badDay))  + '\n';
    }else if (causeCode & badMon){
        cause += this->causeExplanation.at(static_cast<const int>(badMon))  + '\n';
    }else if (causeCode & badYear){
        cause += this->causeExplanation.at(static_cast<const int>(badYear)) + '\n';
    }else if (causeCode & badWday){
        cause += this->causeExplanation.at(static_cast<const int>(badWday)) + '\n';
    }else if (causeCode & badYday){
        cause += this->causeExplanation.at(static_cast<const int>(badYday)) + '\n';
    }else{
        cause = "Unspecified error with date validation";
    }
    return -1;
}

int DateClass::setSec(int sec){
    if(validRange(sec,60)){
        this->sec = sec;
        return dateOk;
    }else{
        return badSec;
    }
}

int DateClass::setMin(int min){
    if(validRange(min,60)){
        this->min = min;
        return dateOk;
    }else{
        return badMin;
    }
}

int DateClass::setHour(int hour){
    if(validRange(sec,24)){
        this->hour = hour;
        return dateOk;
    }else{
        return badHour;
    }
}

int DateClass::setMday(int day){
    if ((this->year != -1)&&(this->mon != -1)){
        if(validRange(sec,getNumberOfDaysInMonth(this->mon))){
            this->mday = day;
            return dateOk;
        }else{
            return badDay;
        }
    }else{
        return badDay;
    }
}

int DateClass::setMon(Months mon){
    if(validRange(static_cast<int>(mon),12)){
        this->mon = mon;
        return dateOk;
    }else{
        return badMon;
    }
}

int DateClass::setYear(int year){
    if (year > 1582){
        this->year = year;
        return dateOk;
    }else{
        return badYear;
    }

}

int DateClass::setWday(){
    if ((this->year != -1)&&(this->mon != -1)){
        int tyear = ((mon==January)||(mon==February)) ? this->year-1 : this->year;
        int tmon=(this->mon+10)%12 +1;
        int y = tyear%100;
        int c = tyear/100;

        this->wday =static_cast<Days>((this->mday + (int)(2.6 * tmon - 0.2) + y + y/4 + c/4 - 2*c)%7);

    }
    return -1;
}

int DateClass::setYday(){
    return -1;
}

int DateClass::setDate(tm date, string &cause){
    int causeCode=0;

    causeCode |= this->setSec(date.tm_sec);
    causeCode |= this->setMin(date.tm_min);
    causeCode |= this->setHour(date.tm_hour);
    causeCode |= this->setMday(date.tm_mday);
    causeCode |= this->setMday(date.tm_mday);
    causeCode |= this->setMon(static_cast<Months>(date.tm_mon));
    causeCode |= this->setYear(date.tm_year);
    causeCode |= this->setMday(date.tm_mday);
    causeCode |= this->setWday();
    causeCode |= this->setYday();

    if (validDate(causeCode,cause)== -1){
        initiateAttributes();
        return -1;
    }else{
        return 1;
    }
}

int DateClass::getSec(){
    return this->sec;
}

int DateClass::getMin(){
    return this->min;
}

int DateClass::getHour(){
    return this->hour;
}

int DateClass::getMday(){
    return this->mday;
}

int DateClass::getMon(){
    return this->mon;
}

int DateClass::getYear(){
    return this->year;
}

int DateClass::getWday(){
    return this->wday;
}

int DateClass::getYday(){
    return this->yday;
}
