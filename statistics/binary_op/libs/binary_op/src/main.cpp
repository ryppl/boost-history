#include <iostream>
#include <libs/binary_op/example/for_each.h>
#include <libs/binary_op/example/find_accumulate.h>
#include <libs/binary_op/example/nn1_accumulate.h>

int main(){

    example_for_each(std::cout);
    example_find_accumulate(std::cout);
    example_nn1_accumulate(std::cout);

    return 0;
}