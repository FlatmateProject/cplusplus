/*
 * XMLfasade_test.cpp
 *
 *  Created on: 16-10-2012
 *      Author: Szymon
 */
#include "gtest/gtest.h"
#include "XMLfasade.h"
#include <TestSlave.h>
#include <TestResource.h>

class XmlFasadeTest: public testing::Test {
protected:

	XmlFasade* testObject;
	TestSlave* testSlave;

	XmlFasadeTest() {
		testObject = NULL;
		testSlave  = NULL;
	}

	virtual void SetUp() {
		testObject = new XmlFasade();
		testSlave = new TestSlave();
	}

	virtual void TearDown() {
		delete testObject;
	}

	void openFileTestPositive(string path) {
		bool faultOccur = false;
		fstream* file = NULL;

		testSlave->createFile(path);

		try {
			file = testObject->openFile(path.c_str());
		} catch (exception &e) {
			faultOccur = true;
		}

		EXPECT_FALSE(faultOccur) << "Exception occur";
		EXPECT_TRUE(file != NULL) << "Wrong value returned";
		EXPECT_FALSE(file->fail() != faultOccur) << "File faulty opened (mismatch of statement)";

		testSlave->deleteFile(path);
	}

	void openFileTestNegative(string path) {
		bool faultOccur = false;
		fstream* file = NULL;

		try {
			file = testObject->openFile(path.c_str());
		} catch (exception &e) {
			faultOccur = true;
		}

		EXPECT_TRUE(faultOccur) << "Exception expected";
		EXPECT_TRUE(file == NULL) << "File faulty opened (mismatch of statement)";
	}

	void readFileTest(string path, string pattern) {
		fstream * file = testSlave->createFile(path, pattern);
		char * actual = NULL;

		try {
			actual = testObject->copyFileContent(file);
		} catch (exception &e) {
			cout << e.what() << endl;
		}
		EXPECT_TRUE(NULL != actual) << "actual == NULL";
		EXPECT_STREQ(pattern.c_str(),actual) << "Strings not equal";

		delete actual;

		testSlave->deleteFile(path);
	}

	void readFileNegativeTest() {
		EXPECT_THROW(testObject->copyFileContent(NULL),xmlException)<<"NULL pointer accepted\n";
	}

	void getFileSizeTest(string path, string pattern) {
		fstream * file = testSlave->createFile(path,pattern);
		unsigned int actual = 0;

		actual = testObject->getFileSize(file);
		EXPECT_EQ(pattern.size(),actual)<<"File size mismatch";

		testSlave->deleteFile(path);
	}

	void getFileSizeTestNegative() {
		EXPECT_THROW(testObject->getFileSize(NULL),xmlException)<<"NULL pointer was accepted, exception should occur\n";
	}

	void getCompilersTest(){
		map<string, string> expected, actual;
		expected["CC"]= "gcc";
		expected["CXX"]= "g++";
		string path = "testFile.xml";

		testSlave->createFile(path.c_str(),TestResource::xmlFileContent);

		try{
			testObject->parseFile(path.c_str());
			actual = testObject->getCompilers();
		}catch(exception &e){
			EXPECT_FALSE(true) << e.what();
		}

		EXPECT_EQ(expected.size(),actual.size())<<"Number of compilers is not equal";

		map<string, string>::iterator it_expected = expected.begin();
		map<string, string>::iterator it_actual = actual.begin();
		for (unsigned int i = 0 ; i < expected.size();i++){
			EXPECT_STREQ((*it_expected).first.c_str(),(*it_actual).first.c_str()) << "Type of compiler is different than expected";
			it_expected++;
			it_actual++;
		}
	}
};

TEST_F(XmlFasadeTest, hello){
	EXPECT_TRUE(true);
}

TEST_F(XmlFasadeTest, openFilePositive) {
	string path = "xmlFile.xml";
	try {
		openFileTestPositive(path.c_str());
	} catch (exception &e) {
		cout << e.what() << endl;
	}
}

TEST_F(XmlFasadeTest, openFileNegative){
	openFileTestNegative("xxx");
}

TEST_F(XmlFasadeTest, copyFileContext) {
	string pattern("<root> <test> pattern </test></root>");
	string path("testFile.txt");

	try {
		readFileTest(path, pattern);
	} catch (exception &e) {
		cout << e.what() << endl;
	}
}

TEST_F(XmlFasadeTest, copyFileConNegative){
	readFileNegativeTest();
}

TEST_F(XmlFasadeTest, getFileSize) {
	try {
		getFileSizeTest("testFile.txt", "12345\n67890");
	} catch (exception &e) {
		cout << e.what() << endl;
	}
}

TEST_F(XmlFasadeTest, getFileSizeNegative){
	getFileSizeTestNegative();
}

TEST_F(XmlFasadeTest, getCompilersTest){
	getCompilersTest();
}
