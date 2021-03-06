/*
 * FilesFinder.h
 *
 *  Created on: 02-10-2012
 *      Author: Szymon
 */

#ifndef FILESFINDER_H_
#define FILESFINDER_H_

using namespace std;

#include <vector>
#include <string>
#include <dirent.h>
#include <iostream>

class FilesFinder {

public:
	FilesFinder();
	virtual ~FilesFinder();

	bool findFilesInPath(string extension, string path = ".");
	void printListOfFile();
	vector<string>::const_iterator getBeginItr();
	vector<string>::const_iterator getEndItr();

#ifndef TEST_BUILD
	private:
#endif
	vector<string> listOfFiles;

	string findExtension(string match);
	std::string createFullPath(string path, char* fileName);
	bool isNotRootOrParentDir(string fileName);
};

#endif /* FILESFINDER_H_ */
