#ifndef MEMBERCLASS_H
#define MEMBERCLASS_H

#include <string>
#include <vector>
#include <ctime>
#include "DateClass.h"

using namespace std;

enum Sex{
    male=1,
    female=0,
    nn=-1
};

class MemberClass
{
public:
    MemberClass();
    ~MemberClass();

private:
    unsigned int id;
    string name;
    string surname;
    Sex sex;

    DateClass birthDate;
    DateClass heavenDate;

    vector<string> picturesPath;

    string relation;
    MemberClass * relationTo;

    unsigned int partnerId;
};

#endif // MEMBERCLASS_H
