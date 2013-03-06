/*
 * TestSlave.cpp
 *
 *  Created on: 21-11-2012
 *      Author: Szymon
 */

#include <FileManager.h>


FileManagerException::FileManagerException() throw(): message("FileManagerException: ") {
}
FileManagerException::~FileManagerException() throw(){
}
FileManagerException FileManagerException::setMessage(const char * message) throw() {
	this->message.append(message);
	return *this;
}
const char* FileManagerException::what() const throw () {
	return message.c_str();
}
void FileManagerException::throwIf(bool condition, const char * message){
	if (condition){
		throw setMessage(message);
	}
}

string FileManager::TESTPATHDIR = "TestPathDir/";

FileManager::FileManager(){

}

FileManager::~FileManager(){
	deleteFiles();
	deleteDirectories();
}

fstream * FileManager::createFile(string path, string text) {
	FileManagerException exception;
	fstream* file;

	createDirectories(extractDirectories(path));

	file = new fstream(path.c_str(), fstream::out | fstream::binary);
	exception.throwIf(file->fail(),"Couldn't create the file");

	file->write(text.c_str(), text.size());
	exception.throwIf(file->fail(),"Couldn't write to file ");
	file->close();

	delete file;

	file = new fstream(path.c_str(), fstream::in | fstream::binary);
	files[string(path)]=file;

	return file;
}

void FileManager::setFileToPath(string path, fstream* file) {
	map<string,fstream*>::iterator it= files.find(path);
	if (it != files.end()){
		if (it->second->is_open()){
			it->second->close();
			delete it->second;
		}
		files[path]=file;
	}
}

void FileManager::closeFile(const map<string, fstream*>::iterator fileDesc) {
	FileManagerException exception;
	if (fileDesc->second != NULL) {
		if (fileDesc->second->is_open()){
			fileDesc->second->close();
		}
		exception.throwIf(fileDesc->second->fail(), string("Coulden't close file "+fileDesc->first).c_str());
		delete fileDesc->second;
	}
}

void FileManager::closeFile(string path) {
	FileManagerException exception;
	map<string, fstream*>::iterator it;
	it = files.find(path);
	exception.throwIf(it == files.end(),path.insert(0,"Couldn't find file to close: ").c_str());
	closeFile(it);
}

void FileManager::closeFiles(){
	map<string, fstream*>::iterator it;
	for (it = files.begin(); it != files.end(); it ++){
		closeFile(it);
	}

}

void FileManager::deleteFile(string path) {
	FileManagerException exception;
	closeFile(path);
	if (remove(path.c_str())) {
		string temp = "Error during deleting file "+path+" "+strerror(errno);
		throw exception.setMessage(temp.c_str());
	}
	files.erase(path);
}

void FileManager::deleteFiles() {
	map<string, fstream*>::iterator it;
	for (it = files.begin(); it != files.end();) {
		deleteFile(it->first);
		it = files.begin();
	}

}

void FileManager::createDirectory(string path) {
	FileManagerException exception;
	string message;

	if (!mkdir(path.c_str())) {
		directories[path]=0;
	} else {
		throw exception.setMessage(message.append("Directory was not created: ").append(path).append(" ").append(strerror(errno)).c_str());
	}

}

void FileManager::createDirectoryIfNotCreated(string path){
	if (directories.find(path) == directories.end()){
				createDirectory(path);
	}
}

void FileManager::createDirectories(string path){
	if (path.empty())
		return;

	int findPosition = 0;
	int i = std::count(path.begin(), path.end(), '/');
	if (i == 0) {
		createDirectoryIfNotCreated(path.substr(0, findPosition));
	} else {
		for (; i > 0; i--) {
			findPosition = path.find('/', findPosition + 1);
			createDirectoryIfNotCreated(path.substr(0, findPosition));
		}
	}
}


void FileManager::deleteDirectory(string path) {
	FileManagerException exception;
	string message;
	if (!rmdir(path.c_str())) {
		directories.erase(path);
	} else {
		throw exception.setMessage(message.append("Directory was not removed: ").append(path).append(" ").append(strerror(errno)).c_str());
	}
}

void FileManager::deleteDirectories() {
	map<string, short>::iterator it;
	for (it = directories.begin(); it != directories.end();) {
		deleteDirectory( it->first);
		it = directories.begin();
	}
}

string FileManager::extractDirectories(string path) {
	size_t dirEnd = path.find_last_of('/');
	if (dirEnd != string::npos) {
		return path.substr(0, dirEnd+1);
	}
	return string();
}
