

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
#include <algorithm>


using namespace std;

class FileManagerException: public exception {

private:
	string message;
public:
	FileManagerException() throw ();
	~FileManagerException() throw ();
	FileManagerException setMessage(const char * message) throw ();
	virtual const char* what() const throw ();
	void throwIf(bool condition, const char * message);
};

//class strLess;

class FileManager{

public:
	FileManager();
	virtual ~FileManager();

	fstream * 	createFile(string path, string text = "");
	void 		setFileToPath(string path, fstream * file);

	static string TESTPATHDIR;


private:
	//bool compareChar(string val1, string val2);
	void closeFile(const map<string, fstream*>::iterator fileDesc);
	string extractDirectories(string path);
	void closeFile(string path);
	void closeFiles();
	void deleteFile(string path);
	void deleteFiles();
	void createDirectory(string path);
	void createDirectoryIfNotCreated(string path);
	void createDirectories(string path);
	void deleteDirectory(string path);
	void deleteDirectories();

private:
	struct Comparator{
		bool operator () (string a, string b){
			return (a.length() > b.length()) || ((a.length() == b.length())&& (a.compare(b) > 0));
		}
	};

	map<string, fstream*> files;
	map<string, short, Comparator> directories;
};



#endif
