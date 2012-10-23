#ifndef DEXDBWRAPPER_H
#define DEXDBWRAPPER_H

#include "DBWrapper.h"

class DexDBWrapper : public DBWrapper
{
public:
    DexDBWrapper();

public:
    int Connect(DBConnectionInf infClass);
    int Initiate();

    int addMember(MemberClass member);
    int delMember(MemberClass member);

    int addRelation(string relation);
    int delRelation(string relation);
    int addRelationTo(string relation, MemberClass member_1, MemberClass member_2);
    int delRelationTo(string relation, MemberClass member_1, MemberClass member_2);

    int findMember(MemberClass member);
    int findByName(MemberClass member);
    int findChildren(MemberClass member);
    int findRealation(MemberClass member_1, MemberClass member_2);
};

#endif // DEXDBWRAPPER_H
