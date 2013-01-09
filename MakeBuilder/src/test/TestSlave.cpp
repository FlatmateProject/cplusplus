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
	file= NULL;
}

TestSlave::~TestSlave(){
	if (file != NULL){
		closeFile();
	}
}

fstream * TestSlave::createFile(string path, string pattern) {
	TestSlaveException exception;
	this->path = path;

	this->file = new fstream(path.c_str(), fstream::out | fstream::binary);
	exception.throwIf(file->fail(),"Couldn't create the file");

	file->write(pattern.c_str(), pattern.size());
	exception.throwIf(file->fail(),"Couldn't write to file ");
	file->close();

	delete file;

	file = new fstream(path.c_str(), fstream::in | fstream::binary);

	return file;
}

void TestSlave::closeFile() {
	TestSlaveException exception;
	if (file != NULL) {
		this->file->close();
		exception.throwIf(file->fail(), "Couldn't close the file");
		delete file;
	}
}

void TestSlave::deleteFile() {
	TestSlaveException exception;
	closeFile();
	if (remove(path.c_str())) {
		string temp = "Error during deleting file "+ path;
		throw exception.setMessage(temp.c_str()) ;
	}
}
