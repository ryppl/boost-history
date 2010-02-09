#include <iostream>
#include <libs/assign/example/cref_list_of2.h>
#include <libs/assign/test/cref_list_of2_speed.h>

int main (int argc, char * const argv[]) {

	example_cref_list_of2(std::cout);
	//test_cref_list_of2_speed(std::cout);
    
    return 0;
}
