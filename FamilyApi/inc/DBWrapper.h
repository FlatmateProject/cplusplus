#ifndef DBWRAPPER_H
#define DBWRAPPER_H

#include "MemberClass.h"
#include "DBConnectionInf.h"
#include <string>


class DBWrapper{

public:
    virtual int Connect(DBConnectionInf infClass)=0;
    virtual int Initiate()=0;

    virtual int addMember(MemberClass member)=0;
    virtual int delMember(MemberClass member)=0;

    virtual int addRelation(string relation)=0;
    virtual int delRelation(string relation)=0;
    virtual int addRelationTo(string relation, MemberClass member_1, MemberClass member_2)=0;
    virtual int delRelationTo(string relation, MemberClass member_1, MemberClass member_2)=0;

    virtual int findMember(MemberClass member)=0;
    virtual int findByName(MemberClass member)=0;
    virtual int findChildren(MemberClass member)=0;
    virtual int findRealation(MemberClass member_1, MemberClass member_2)=0;

};

#endif // DBWRAPPER_H
