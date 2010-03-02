#include <iostream>
#include <libs/assign/example/ref_list_of.h>
#include <libs/assign/test/speed.h>
#include <libs/assign/test/speed2.h>

int main (int argc, char * const argv[]) {

    example_ref_list_of(std::cout);
    test_speed(std::cout);
    test_speed2(std::cout);

    return 0;

}
