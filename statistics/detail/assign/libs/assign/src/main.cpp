#include <iostream>
#include <libs/assign/example/cref_list_of2.h>
#include <libs/assign/test/speed.h>
#include <libs/assign/test/speed_csv.h>
//#include <libs/assign/test/static_list_of_auto_size.h>

int main (int argc, char * const argv[]) {

	example_cref_list_of(std::cout);
	test_speed(std::cout);
	test_speed_csv(std::cout);
//	check_static_list_of_auto_size();

	return 0;

}
