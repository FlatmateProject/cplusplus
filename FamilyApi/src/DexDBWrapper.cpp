#include "DexDBWrapper.h"

DexDBWrapper::DexDBWrapper()
{
}

int DexDBWrapper::Connect(DBConnectionInf infClass){
	return 0;
}
int DexDBWrapper::Initiate(){
	return 0;
}

int DexDBWrapper::addMember(MemberClass member){
	return 0;
}
int DexDBWrapper::delMember(MemberClass member){
	return 0;
}

int DexDBWrapper::addRelation(string relation){
	return 0;
}
int DexDBWrapper::delRelation(string relation){
	return 0;
}
int DexDBWrapper::addRelationTo(string relation, MemberClass member_1, MemberClass member_2){
	return 0;
}
int DexDBWrapper::delRelationTo(string relation, MemberClass member_1, MemberClass member_2){
	return 0;
}

int DexDBWrapper::findMember(MemberClass member){
	return 0;
}
int DexDBWrapper::findByName(MemberClass member){
	return 0;
}
int DexDBWrapper::findChildren(MemberClass member){
	return 0;
}
int DexDBWrapper::findRealation(MemberClass member_1, MemberClass member_2){
	return 0;
}
