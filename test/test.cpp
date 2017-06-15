/*
Author: QIN Shuo
Date:   2017/6/2
Description:
	Main file for test
*/



#include <string>

void test_reader(std::string folder_name);
int test_seedwidgets();
int test_3d_curve();
int test_3d_curve_class();
int test_vertebra_stack();
int test_orientation();

int main(int argc, char** argv)
{
	//test_vertebra_stack();
	std::string dir_name = "F:/Newdata_5_31/June/16-06-01_Y5726156";
	//test_reader(dir_name);
	//test_seedwidgets();

	//test_3d_curve();
	test_3d_curve_class();
	test_orientation();

	return 0;
}







