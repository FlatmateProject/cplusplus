#include "gtest/gtest.h"
#include <TestSlave.h>
#include "FilesFinder.h"


class FileFinderTest: public testing::Test {
protected:
	FilesFinder* finder;
	TestSlave* testSlave;

	FileFinderTest(){
		finder= NULL;
		testSlave = NULL;
	}

	virtual void SetUp() {
		finder = new FilesFinder();
		testSlave = new TestSlave();
	}

	virtual void TearDown() {
		delete finder;
	}

	virtual void findFiles(){
		string path("./eszyste");
		testSlave->createDirectory(path);
		finder->findFilesInPath("h","D:/eclipse_cpp/workspace/MakeBuilder/Test/testFiles");
	}

	void compareSizeOfList(){
		EXPECT_EQ((unsigned int)4,finder->listOfFiles.size())<<"Number of elements is not correct";
	}
};

TEST_F(FileFinderTest, hello){
	EXPECT_TRUE(true);
}

TEST_F(FileFinderTest, isNotNull){
	EXPECT_TRUE(finder != NULL)<< "Not initiated";
}

TEST_F(FileFinderTest, CheckNumberOfFiles){
	findFiles();
	compareSizeOfList();
}






