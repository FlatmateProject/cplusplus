#include "gtest/gtest.h"
#include <FileManager.h>
#include "FilesFinder.h"


class FileFinderTest: public testing::Test {
protected:
	FilesFinder* testedObject;
	FileManager* testSlave;

	FileFinderTest(){
		testedObject= NULL;
		testSlave = NULL;
	}

	virtual void SetUp() {
		testedObject = new FilesFinder();
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

TEST_F(FileFinderTest, hello){
	EXPECT_TRUE(true);
}

TEST_F(FileFinderTest, isNotNull){
	EXPECT_TRUE(testedObject != NULL)<< "Not initiated";
}

TEST_F(FileFinderTest, CheckNumberOfFiles){

	testSlave->createFile(FileManager::TESTPATHDIR+"dir1/file1.h");
	testSlave->createFile(FileManager::TESTPATHDIR+"dir1/file2.h");
	testSlave->createFile(FileManager::TESTPATHDIR+"dir1/file2.cpp");
	testSlave->createFile(FileManager::TESTPATHDIR+"dir1/dir2/file3.h");
	testSlave->createFile(FileManager::TESTPATHDIR+"dir1/dir2/file3.cpp");
	testSlave->createFile(FileManager::TESTPATHDIR+"dir1/dir2/dir3/file2.h");

	testedObject->findFilesInPath("h",FileManager::TESTPATHDIR);

	EXPECT_EQ((unsigned int)4,testedObject->listOfFiles.size())<<"Number of elements is not correct";
}






