/*
 * XMLfasade.cpp
 *
 *  Created on: 16-10-2012
 *      Author: Szymon
 */

#include "XMLfasade.h"



xmlFasade::xmlFasade() {
	// TODO Auto-generated constructor stub

}

xmlFasade::~xmlFasade() {
	// TODO Auto-generated destructor stub
}



int xmlFasade::readFile(const char * path) {

	ifstream file(path);//,istream::binary);
	if (file.fail())
		return -1;

	file.seekg(0, ios::end);
	size_t length = file.tellg();
	file.seekg(0, ios::beg);
	char * buffer = new char[length + 1];
	file.read(&buffer[0], length);
	buffer[length] = '\0';

	file.close();
	cout << &buffer[0]<<endl;

	try {
		dom.parse<0>(&buffer[0]);
		root = dom.first_node("root");
	} catch (parse_error &e) {
		cout << e.what() << endl;
		return -1;
	}
	cout<< root->first_node()->name() <<endl;
	return 1;
}
