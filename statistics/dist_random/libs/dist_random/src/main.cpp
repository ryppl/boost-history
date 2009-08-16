//////////////////////////////////////////////////////////////////////////////
// dist_random::src::main.cpp                                               //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <libs/dist_random/example/chi_squared.h>
#include <libs/dist_random/example/location_scale_students_t.h>
#include <libs/dist_random/example/generate_n.h>
#include <libs/dist_random/example/sample.h>

int main(){

    example_chi_squared(std::cout); 
    std::cout << std::endl;
    example_location_scale_students_t(std::cout);
    std::cout << std::endl;
    example_generate_n(std::cout);
    std::cout << std::endl;
    example_sample(std::cout);

    return 0;
}


