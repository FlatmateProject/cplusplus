/*
 * TestSlave.cpp
 *
 *  Created on: 21-11-2012
 *      Author: Szymon
 */

#include <TestSlave.h>


TestSlaveException::TestSlaveException() throw(): message("TestSlaveException: ") {
}
TestSlaveException::~TestSlaveException() throw(){
}
TestSlaveException TestSlaveException::setMessage(const char * message) throw() {
	this->message.append(message);
	return *this;
}
const char* TestSlaveException::what() const throw () {
	return message.c_str();
}
void TestSlaveException::throwIf(bool condition, const char * message){
	if (condition){
		throw setMessage(message);
	}
}


TestSlave::TestSlave(){

}

TestSlave::~TestSlave(){
	deleteFiles();
}

fstream * TestSlave::createFile(string path, string pattern) {
	TestSlaveException exception;
	fstream* file;

	file = new fstream(path.c_str(), fstream::out | fstream::binary);
	exception.throwIf(file->fail(),"Couldn't create the file");

	file->write(pattern.c_str(), pattern.size());
	exception.throwIf(file->fail(),"Couldn't write to file ");
	file->close();

	delete file;

	file = new fstream(path.c_str(), fstream::in | fstream::binary);
	files[string("./"+path)]=file;

	return file;
}

void TestSlave::closeFile(const map<string, fstream*>::iterator fileDesc) {
	TestSlaveException exception;
	if (fileDesc->second != NULL) {
		fileDesc->second->close();
//		exception.throwIf(fileDesc->second->fail(), fileDesc->first.insert(0,"Couldn't close the file: ").c_str());
		delete fileDesc->second;
	}
}

void TestSlave::closeFile(string path) {
	TestSlaveException exception;
	map<string, fstream*>::iterator it;
	it = files.find(path);
	exception.throwIf(it == files.end(),path.insert(0,"Couldn't find file to close: ").c_str());
	closeFile(it);
}

void TestSlave::closeFiles(){
	map<string, fstream*>::iterator it;
	for (it = files.begin(); it != files.end(); it ++){
		closeFile(it);
	}

}

void TestSlave::deleteFile(string path) {
	TestSlaveException exception;
	closeFile(path);
	if (remove(path.c_str())) {
		string temp = "Error during deleting file "+ path;
		throw exception.setMessage(temp.c_str());
	}
	files.erase(path);
}

void TestSlave::deleteFiles() {
	map<string, fstream*>::iterator it;
	for (it = files.begin(); it != files.end();) {
		deleteFile(it->first);
		it = files.begin();
	}

}

void TestSlave::createDirectory(string path) {
	TestSlaveException exception;
	string message;
	if (!mkdir(path.c_str())) {
		directories.push_back(path);
	} else {
		throw exception.setMessage(message.append("Directory was not created: ").append(path).append(" ").append(strerror(errno)).c_str());
	}

}
void TestSlave::deleteDirectory(string path) {
	TestSlaveException exception;
	string message;
	if (!rmdir(path.c_str())) {
		//directories.erase(path);
	} else {
		throw exception.setMessage(message.append("Directory was not removed: ").append(path).append(" ").append(strerror(errno)).c_str());
	}
}
void TestSlave::deleteDirectories() {


}
