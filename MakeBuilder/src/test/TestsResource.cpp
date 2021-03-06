/*
 * TestsResource.cpp
 *
 *  Created on: 13-11-2012
 *      Author: Szymon
 */
#include <TestResource.h>

const string TestResource::xmlFileContent =
				"<?xml version=\"1.0\"?> \
    <root>\
	<compilers>\
		<ansic alias=\"CC\">gcc</ansic> \
		<cplusplus alias=\"CXX\">g++</cplusplus>\
	</compilers>\
	<linkers>\
		<linker alias=\"LINK\">ar</linker>\
	</linkers>\
	<declarations>\
		<define alias=\"DEFINE\"></define>\
	</declarations>\
	<flags>\
		<subflag alias=\"CFLAGS\" relatedto=\"ansic\">\
			<flag>O2</flag>\
			<flag>Wall</flag>\
		</subflag>\
		<subflag alias=\"CXXFLAGS\" relatedto=\"cplusplus\">\
			<flag>O2</flag>\
			<flag>frtti</flag> \
			<flag>fexceptions</flag> \
			<flag>mthreads</flag>\
			<flag>Wall</flag>\
		</subflag>\
	</flags>\
	<incpaths alias=\"INCPATH\">\
		<path>.</path>\
		<path>./inc</path>\
	</incpaths>\
	<libpath alias=\"LIBPATH\">\
		<lib>./lib</lib>\
	</libpath>\
	<libs alias=\"LIBS\">\
		<lib>mingw3</lib>\
	</libs>\
	<mkdir alias=\"MKDIR\">mkdir</mkdir>\
	<rmdir alias=\"RMDIR\">rm -rfv</rmdir>\
	<build>\
		<release alias=\"RELEAS\">release</release>\
		<target alias=\"TARGET\">treeAPI.a</target>\
		<destdir>target</destdir>\
		<linker>linker</linker>\
	</build>\
</root>";
