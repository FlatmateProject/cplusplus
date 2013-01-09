

#ifndef TESTSLAVE_
#define TESTSLAVE_

#include <fstream>
#include <string>
#include <exception>

using namespace std;

class TestSlaveException: public exception {

private:
	string message;
public:
	TestSlaveException() throw ();
	~TestSlaveException() throw ();
	TestSlaveException setMessage(const char * message) throw ();
	virtual const char* what() const throw ();
	void throwIf(bool condition, const char * message);
};

class TestSlave{

public:
	TestSlave();
	virtual ~TestSlave();

	fstream * createFile(string path, string pattern = "NULL");
	void closeFile();
	void deleteFile();

private:
	fstream * file;
	string path;

};



#endif
