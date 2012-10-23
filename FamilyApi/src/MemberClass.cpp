#include "MemberClass.h"

MemberClass::MemberClass()
{
    relationTo=NULL;
    sex=nn;
}

MemberClass::~MemberClass()
{
    if (!relationTo){
        delete relationTo;
    }
}
