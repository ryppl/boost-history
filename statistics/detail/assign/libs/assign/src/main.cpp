#include <iostream>
#include <libs/assign/example/ref_list_of.h>
#include <libs/assign/example/ref_csv.h>
#include <libs/assign/test/speed.h>
//#include <libs/assign/test/speed2.h>
//#include <libs/assign/test/ref_list_of.h>

int main (int argc, char * const argv[]) {

    example_ref_list_of(std::cout);
    example_ref_csv(std::cout);
    test_speed(std::cout);
    //test_speed2(std::cout);
    //check_ref_list_of();

    return 0;

}
