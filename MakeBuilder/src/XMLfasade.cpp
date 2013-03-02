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

void xmlException::throwIf(bool condition, const char * message){
	if (condition){
		throw setMessage(message);
	}
}


XmlFasade::XmlFasade() {
	root = NULL;
}

XmlFasade::~XmlFasade() {

}

void XmlFasade::checkFileFlags(fstream* file, string message) {
	if (file->fail() || file->eof()) {
		xmlException exception;
		message.append(" [");
		message.append(file->eofbit ? " eofbit" : "");
		message.append(file->failbit ? " failbit" : "");
		message.append(file->badbit ? " badbit" : "");
		message.append(" ]\n");
		throw exception.setMessage(message.c_str());
	}
}


size_t XmlFasade::getFileSize(fstream* file) {
	if (file == NULL) {
		xmlException exception;
		throw exception.setMessage("NULL pointer of file descriptor");
	}
	file->seekg(0, ios::end);
	size_t length = file->tellg();
	file->seekg(0, ios::beg);
	return length;
}

/**
 *  Copy file content to char buffer.
 *  Return pointer to buffer.
 */
char* XmlFasade::copyFileContent(fstream* file) {
	if (file == NULL){
		xmlException exception;
		throw exception.setMessage("NULL pointer of file descriptor");
	}

	size_t length = getFileSize(file);
	char* buffer = new char[length + 1];
	try {
		file->read(&buffer[0], length);
	} catch (exception &e) {
		xmlException exception;
		string message(e.what());
		exception.setMessage(message.append("Read file error").c_str());
		throw exception;
	}
	buffer[length] = '\0';
	checkFileFlags(file, "Read file error");
	return buffer;
}

fstream * XmlFasade::openFile(const char* path) {
	fstream * file =new fstream(path, fstream::in | fstream::binary);
	if (file->fail()) {
		xmlException exception;
		throw exception.setMessage("File not found");
		return NULL;
	}
	return file;
}


void XmlFasade::parseFile(const char * path) {
	fstream * file = openFile(path);
	char * buffer = copyFileContent(file);
	file->close();
	if (file->fail()) {
		cout << "close failed\n";
	}
	dom.parse<0>(&buffer[0]);
	delete file;
}

map<string, string> XmlFasade::getCompilers() {

	xmlException exception;
	map<string, string> list;

	xml_node<> *root = dom.first_node("root");
	exception.throwIf(root == NULL, "Node 'root' not found");

	xml_node<> *compilers = root->first_node("compilers");
	exception.throwIf(compilers == NULL, "Node 'compilers' not found");

	xml_node<> *node = compilers->first_node();
	while(node != NULL){
		list[node->first_attribute("alias")->value()]=node->value();
		node = node->next_sibling();
	}
	return list;
}
