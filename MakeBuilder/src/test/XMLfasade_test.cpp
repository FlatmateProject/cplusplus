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

	}

	virtual void SetUp() {
		testObject = new XmlFasade();
		testSlave = new TestSlave();
	}

	virtual void TearDown() {
		delete testObject;
	}

	void openFileTest(const char* path, bool positiveTest){
		bool faultOccur = false;
		fstream* file = NULL;

		try{
			file = testObject->openFile(path);
		}catch(exception &e){
			faultOccur = true;
		}

		if (positiveTest){
			EXPECT_FALSE(faultOccur) << "Exception occur";
			EXPECT_TRUE(file != NULL) << "Wrong value returned";
			EXPECT_FALSE(file->fail() != faultOccur) << "File faulty opened (mismatch of statement)";

			file->close();
			delete file;
		}else{
			EXPECT_TRUE(faultOccur) << "Exception expected";
			EXPECT_TRUE(file == NULL) << "File faulty opened (mismatch of statement)";
		}
	}

	void readFileTest(string path, string pattern){
		fstream * file = testSlave->createFile(path,pattern);
		char * actual = NULL;

		try {
			actual = testObject->copyFileContent(file);
		} catch (exception &e) {
			cout << e.what()<<endl;
		}
		EXPECT_TRUE(NULL != actual) << "actual == NULL";
		EXPECT_STREQ(pattern.c_str(),actual) << "String not equal";

		delete actual;

		testSlave->deleteFile();
	}

	void readFileNegativeTest() {
		EXPECT_THROW(testObject->copyFileContent(NULL),xmlException)<<"NULL pointer accepted\n";
	}

	void getFileSizeTest(string path, string pattern) {
		fstream * file = testSlave->createFile(path,pattern);
		unsigned int actual = 0;

		actual = testObject->getFileSize(file);

		EXPECT_EQ(pattern.size(),actual)<<"File size mismatch";

		testSlave->deleteFile();
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

TEST_F(XmlFasadeTest, openFilePositive){
	string path = "xmlFile.xml";
	testSlave->createFile(path);
	openFileTest(path.c_str(),true);
}

TEST_F(XmlFasadeTest, openFileNegative){
	openFileTest("xxx",false);
}

TEST_F(XmlFasadeTest, copyFileContext){
	string pattern("<root> <test> pattern </test></root>");
	string path("testFile.txt");

	readFileTest(path,pattern);
}

TEST_F(XmlFasadeTest, copyFileConNegative){
	readFileNegativeTest();
}

TEST_F(XmlFasadeTest, getFileSize){
	getFileSizeTest("testFile.txt","12345\n67890");
}

TEST_F(XmlFasadeTest, getFileSizeNegative){
	getFileSizeTestNegative();
}

TEST_F(XmlFasadeTest, getCompilersTest){
	getCompilersTest();
}
