

#ifndef TESTSLAVE_
#define TESTSLAVE_

#include <fstream>
#include <string>
#include <cstring>
#include <exception>
#include <io.h>
#include <map>
#include <vector>
#include <errno.h>
#include <iostream>


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

//class strLess;

class TestSlave{

public:
	TestSlave();
	virtual ~TestSlave();

	fstream * createFile(string path, string pattern = "");
	void closeFile(string path);
	void closeFiles();
	void deleteFile(string path);
	void deleteFiles();
	void createDirectory(string path);
	void deleteDirectory(string path);
	void deleteDirectories();

private:
	bool compareChar(string val1, string val2);
	void closeFile(const map<string, fstream*>::iterator fileDesc);


private:
	map<string, fstream*> files;
	vector<string> directories;
};



#endif
