#include <iostream>

#include "gtest/gtest.h"
#include "TestSlave.h"
#include <exception>

/*
GTEST_API_ int main(int argc, char **argv) {
  std::cout << "Running main() from gtest_main.cc\n";

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
*/

using namespace std;

int main (int argc, char ** argv){
	TestSlave * slave = new TestSlave();

	try {
		slave->createFile("pliczek.h");

		slave->createDirectory("pathDir");

		slave->createFile("pathDir/pliczek2.h");

		system("pause");

		delete slave;



	} catch (exception &e) {
		cout<< e.what()<<endl;
	}
}
