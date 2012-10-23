#include "gtest/gtest.h"
#include "FilesFinder.h"


class FileFinderTest: public testing::Test {
protected:
	FilesFinder* finder;

	FileFinderTest(){

	}

	virtual void SetUp() {
		finder = new FilesFinder();
	}

	virtual void TearDown() {
		delete finder;
	}

	virtual void findFiles(){
		finder->findFilesInPath("h","D:/eclipse_cpp/workspace/MakeBuilder/Test/testFiles");
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
	EXPECT_EQ(4,finder->getNumberOfElement())<<"Number of elements is not correct";
}






