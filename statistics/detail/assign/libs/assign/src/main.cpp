#include <iostream>
#include <libs/assign/example/cref_list_of2.h>
#include <libs/assign/test/cref_list_of2_speed.h>
//#include <libs/assign/test/static_list_of_auto_size.h>

int main (int argc, char * const argv[]) {

	example_cref_list_of(std::cout);
//	test_cref_copy_list_of_speed(std::cout);
//	check_static_list_of_auto_size();

	return 0;

}
