/*
 * XMLfasade.h
 *
 *  Created on: 16-10-2012
 *      Author: Szymon
 */

#ifndef XMLFASADE_H_
#define XMLFASADE_H_

#include <rapidxml.hpp>
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
	xmlException()throw();
	~xmlException()throw();
	xmlException setMessage(const char * message) throw();
	virtual const char* what() const throw ();
};

class xmlFasade {
public:
	xmlFasade();
	virtual ~xmlFasade();

	void parseFile(const char * path);
	map<string, string> getCompilersAndLinker();
	map<string, string> getFlags();
	map<string, string> getCompilerFlagRelation();
private:
	xml_document<> dom;
	xml_node<> * root;
	fstream * openFile(const char* path);
	char* copyFileContent(fstream& file);
	size_t getFileSize(fstream& file);
};


#endif /* XMLFASADE_H_ */
