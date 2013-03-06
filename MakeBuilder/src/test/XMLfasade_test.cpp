/*
 * XMLfasade_test.cpp
 *
 *  Created on: 16-10-2012
 *      Author: Szymon
 */
#include "gtest/gtest.h"
#include "XMLfasade.h"
#include <FileManager.h>
#include <TestResource.h>

class XmlFasadeTest: public testing::Test {
protected:

	XmlFasade* testedObject;
	FileManager* testSlave;

	XmlFasadeTest() {
		testedObject = NULL;
		testSlave  = NULL;
	}

	virtual void SetUp() {
		testedObject = new XmlFasade();
		testSlave = new FileManager();
	}

	virtual void TearDown() {
		delete testedObject;
		delete testSlave;
	}
};

//=========================================================================================================================
//     TEST CASES START      ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//=========================================================================================================================

TEST_F(XmlFasadeTest, hello){
	EXPECT_TRUE(true);
}

TEST_F(XmlFasadeTest, openFilePositive) {
	string path = FileManager::TESTPATHDIR + "xmlFile.xml";
	testSlave->createFile(path);

	fstream* file = NULL;

	try {
		file = testedObject->openFile(path.c_str());
	} catch (exception &e) {
		FAIL()<< e.what();
	}

	testSlave->setFileToPath(path, file);

	EXPECT_FALSE(file == NULL) << "Wrong value returned";
	EXPECT_FALSE(file->fail()) << "File faulty opened (mismatch of statement)";
}

TEST_F(XmlFasadeTest, openFileNegative){
	string path("xxx");
	fstream* file = NULL;

	EXPECT_THROW(file = testedObject->openFile(path.c_str()), xmlException);
	EXPECT_TRUE(file == NULL) << "File faulty opened (mismatch of statement)";
}

TEST_F(XmlFasadeTest, copyFileContext) {
	string text("<root> <test> pattern </test></root>");
	string path(FileManager::TESTPATHDIR + "testFile.txt");

	fstream * file = testSlave->createFile(path, text);
	char * actual = NULL;

	try {
		actual = testedObject->copyFileContent(file);
	} catch (exception &e) {
		cout << e.what() << endl;
	}
	EXPECT_FALSE(actual == NULL) << "actual == NULL";
	EXPECT_STREQ(text.c_str(),actual) << "Strings not equal";

	delete actual;
}

TEST_F(XmlFasadeTest, copyFileConNegative){
	EXPECT_THROW(testedObject->copyFileContent(NULL),xmlException)<<"NULL pointer accepted\n";
}

TEST_F(XmlFasadeTest, getFileSize) {;
	string path(FileManager::TESTPATHDIR + "testFile.txt");
	string text("12345\n67890");

	fstream * file = testSlave->createFile(path, text);
	unsigned int actual = 0;

	actual = testedObject->getFileSize(file);
	EXPECT_EQ(text.size(),actual) << "File size mismatch";
}

TEST_F(XmlFasadeTest, getFileSizeNegative){
	EXPECT_THROW(testedObject->getFileSize(NULL),xmlException)<<"NULL pointer was expected, exception should occur\n";
}

TEST_F(XmlFasadeTest, getCompilersTest) {
	string path = FileManager::TESTPATHDIR + "testFile.xml";
	testSlave->createFile(path.c_str(), TestResource::xmlFileContent);

	map<string, string> expected, actual;
	expected["CC"] = "gcc";
	expected["CXX"] = "g++";

	try {
		testedObject->parseFile(path.c_str());
		actual = testedObject->getCompilers();
	} catch (exception &e) {
		EXPECT_FALSE(true) << e.what();
	}

	EXPECT_EQ(expected.size(),actual.size())
			<< "Number of compilers is not equal";

	map<string, string>::iterator it_expected = expected.begin();
	map<string, string>::iterator it_actual = actual.begin();
	for (unsigned int i = 0; i < expected.size(); i++) {
		EXPECT_STREQ((*it_expected).first.c_str(),(*it_actual).first.c_str())
				<< "Type of compiler is different than expected";
		it_expected++;
		it_actual++;
	}
}
