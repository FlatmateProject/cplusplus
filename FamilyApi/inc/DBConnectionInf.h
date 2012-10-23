#ifndef DBCONNECTIONINF_H
#define DBCONNECTIONINF_H

#include <string>

using namespace std;

class DBConnectionInf{

private:
    string ip;
    string user;
    string password;
    string dbName;

public:
    void setIp(string ip){
        this->ip=ip;
    }

    string getIp(){
        return this->ip;
    }

    void setUser(string user){
        this->user=user;
    }

    string getUser(){
        return this->user;
    }

    void setPassword(string password){
        this->password=password;
    }

    string getPassword(){
        return this->password;
    }

    void setDbName(string dbName){
        this->dbName=dbName;
    }

    string getDbName(){
        return this->dbName;
    }

};

#endif // DBCONNECTIONINF_H
