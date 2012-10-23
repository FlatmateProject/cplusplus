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



using namespace rapidxml;
using namespace std;

class xmlFasade {
public:
	xmlFasade();
	virtual ~xmlFasade();

	void validFile(const char * path);
	map<string, string> getCompilersAndLinker();
	map<string, string> getFlags();
	map<string, string> getCompilerFlagRelation();
private:
	xml_document<> dom;
	xml_node<> * root;
};


#endif /* XMLFASADE_H_ */
