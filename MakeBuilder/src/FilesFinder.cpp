/*
 * FilesFinder.cpp
 *
 *  Created on: 02-10-2012
 *      Author: Szymon
 */

#include "FilesFinder.h"

FilesFinder::FilesFinder() {
}

FilesFinder::~FilesFinder() {
}


string FilesFinder::findExtension(string fileName) {
	size_t found = fileName.find_last_of('.');
	return fileName.substr(found+1);
}

std::string FilesFinder::createFullPath(string path, char* fileName) {
	path = path.compare(".") ? path + "/" : "";
	return (path + fileName);
}

bool FilesFinder::isNotRootOrParentDir(string fileName) {
	return (fileName != ".") && (fileName != "..");
}

bool FilesFinder::findFilesInPath(string extension, string path){
	DIR *dir = opendir(path.c_str());
	if(!dir)
	{
		return false;
	}

	struct dirent* node;
	while ((node = readdir(dir)) != NULL) {
		bool isDir = true;
		string fileName(node->d_name);

		if (isNotRootOrParentDir(fileName))
		{
			isDir = findFilesInPath(extension, createFullPath(path, node->d_name));
		}

		if ((!isDir)&&(extension == findExtension(fileName)))
		{
			this->listOfFiles.push_back(createFullPath(path, node->d_name));
		}
	}
	closedir(dir);
	return true;
}

void FilesFinder::printListOfFile(){
	cout << endl;
	for (unsigned int i = 0; i < listOfFiles.size() ; i++){
		cout << listOfFiles[i] << " \\" << endl;
	}
}

vector<string>::const_iterator FilesFinder::getBeginItr(){
	return listOfFiles.begin();
}

vector<string>::const_iterator FilesFinder::getEndItr(){
	return listOfFiles.end();
}

int FilesFinder::getNumberOfElement(){
	return listOfFiles.size();
}
