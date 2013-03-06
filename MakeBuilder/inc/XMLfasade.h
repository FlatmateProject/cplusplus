/*
 * XMLfasade.h
 *
 *  Created on: 16-10-2012
 *      Author: Szymon
 */

#ifndef XMLFASADE_H_
#define XMLFASADE_H_

#include <rapidxml.hpp>
#include <rapidxml_print.hpp>
#include <fstream>
#include <string>
#include <exception>
#include <map>
#include <iostream>

using namespace rapidxml;
using namespace std;

class xmlException: public exception {

private:
	string message;
public:
	xmlException() throw ();
	~xmlException() throw ();
	xmlException setMessage(const char * message) throw ();
	virtual const char* what() const throw ();
	void throwIf(bool condition, const char * message);
};

class XmlFasade {

public:
	XmlFasade();
	virtual ~XmlFasade();
	void parseFile(const char* path);
	map<string, string> getCompilers();
	map<string, string> getLinker();
	map<string, string> getFlags();
	map<string, string> getCompilerFlagRelation();

#ifndef TEST_BUILD
	private:
#endif
	xml_document<> dom;
	xml_node<>* root;
	void checkFileFlags(fstream* file, string message);
	fstream * openFile(const char* path);
	char* copyFileContent(fstream* file);
	size_t getFileSize(fstream* file);
};

#endif /* XMLFASADE_H_ */
