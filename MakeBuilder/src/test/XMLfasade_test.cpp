/*
 * XMLfasade_test.cpp
 *
 *  Created on: 16-10-2012
 *      Author: Szymon
 */
#include "gtest/gtest.h"
#include "XMLfasade.h"


class XmlFasadeTest: public testing::Test {
protected:

	XmlFasade* testObject;

	XmlFasadeTest() {

	}

	virtual void SetUp() {
		testObject = new XmlFasade();
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

	fstream * createFile(string path, string pattern){
		fstream * file =new fstream(path.c_str(),fstream::out | fstream::binary );
		EXPECT_FALSE(file->fail())<< "Couldn't create the file";
		file->write(pattern.c_str(), pattern.size());
		EXPECT_FALSE(file->fail())<< "Couldn't write to file ";
		file->close();

		delete file;

		file =new fstream(path.c_str(), fstream::in | fstream::binary );

		return file;
	}



	void readFileTest(string path, string pattern){
		fstream * file = createFile(path,pattern);
		char * actual = NULL;

		try {
			actual = testObject->copyFileContent(file);
		} catch (exception &e) {
			cout << e.what()<<endl;
		}
		EXPECT_TRUE(NULL != actual) << "actual == NULL";
		EXPECT_STREQ(pattern.c_str(),actual) << "String not equal";

		delete actual;

		deleteFile(file, path);
	}

	void deleteFile(fstream * file, string path) {
		file->close();
		ASSERT_FALSE(file->fail())<<"Couldn't close the file";
		delete file;

		if (remove(path.c_str())){
				cout << "Error during deleting file "<<path<<endl;
		}
	}

	void readFileNegativeTest() {
		EXPECT_THROW(testObject->copyFileContent(NULL),xmlException)<<"NULL pointer accepted\n";
	}
};

TEST_F(XmlFasadeTest, hello){
	EXPECT_TRUE(true);
}

TEST_F(XmlFasadeTest, openFilePositive){
	openFileTest("D:/eclipse_cpp/workspace/MakeBuilder/xml/xmlFile.xml",true);
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




