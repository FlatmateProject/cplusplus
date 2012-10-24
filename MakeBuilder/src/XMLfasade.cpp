/*
 * XMLfasade.cpp
 *
 *  Created on: 16-10-2012
 *      Author: Szymon
 */

#include "XMLfasade.h"

using namespace std;

xmlException::xmlException() throw(): message("xmlException: ") {

}

xmlException::~xmlException() throw(){

}

xmlException xmlException::setMessage(const char * message) throw() {
	this->message.append(message);
	return *this;
}

const char* xmlException::what() const throw () {
	return message.c_str();
}



xmlFasade::xmlFasade() {
	// TODO Auto-generated constructor stub

}

xmlFasade::~xmlFasade() {
	// TODO Auto-generated destructor stub
}



size_t xmlFasade::getFileSize(fstream& file) {
	file.seekg(0, ios::end);
	size_t length = file.tellg();
	file.seekg(0, ios::beg);
	return length;
}

char* xmlFasade::copyFileContent(fstream& file) {
	size_t length = getFileSize(file);

	char* buffer = new char[length + 1];
	file.read(&buffer[0], length);
	buffer[length] = '\0';
	return buffer;
}

fstream * xmlFasade::openFile(const char* path) {
	fstream * file =new fstream(path,fstream::out | fstream::binary);
	if (file->fail()) {
		xmlException exception;
		throw exception.setMessage("File not found");
	}
	return file;
}

void xmlFasade::parseFile(const char * path) {
	fstream * file = openFile(path);
	char * buffer = copyFileContent(*file);
	file->close();
	dom.parse<0>(&buffer[0]);
	delete file;
}
