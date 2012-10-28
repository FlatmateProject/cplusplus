//============================================================================
// Name        : MakeBuilder.cpp
// Author      : Szymon Stefanczyk
// Version     :
// Copyright   : Ask me if you want to use it
// Description : Hello World in C++, Ansi-style
//============================================================================
using namespace std;


#include <iostream>
#include <FilesFinder.h>
#include <XMLfasade.h>




int openFile(char * filePath){

	return 0;
}

int main(int argc, char* argv[]) {

	xmlFasade xmlReader;
	try {
		xmlReader.parseFile("D:/eclipse_cpp/workspace/MakeBuilder/xml/xmlFile.xml");
		xmlReader.getCompilers();
	} catch (exception& e) {
		cout << e.what() << endl;
	}

	/*FilesFinder finder;

	 finder.findFilesInPath("h",".");
	 finder.printListOfFile();

	 char * basefile;
	 char * extendfile;

	 if (argc != 2){
	 cout<<" Wrong number of arguments"<<endl;
	 }*/

}
