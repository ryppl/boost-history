#include <iostream>
#include <libs/assign/example/ref_list_of.h>
//#include <libs/assign/test/speed.h>
//#include <libs/assign/test/speed_csv.h>
#include <libs/assign/test/speed2.h>
//#include <libs/assign/test/static_list_of_auto_size.h>

int main (int argc, char * const argv[]) {

	example_ref_list_of(std::cout);
	//test_speed(std::cout);
    test_speed2(std::cout);
//	check_static_list_of_auto_size();

	return 0;

}
