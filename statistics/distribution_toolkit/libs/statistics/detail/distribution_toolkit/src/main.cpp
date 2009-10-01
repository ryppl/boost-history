//////////////////////////////////////////////////////////////////////////////
// distribution_toolkit::src::main.h                                        //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <libs/statistics/detail/distribution_toolkit/example/distribution_function.h>
#include <libs/statistics/detail/distribution_toolkit/example/random.h>
#include <libs/statistics/detail/distribution_toolkit/test/unnormalized_functions.cpp>
#include <libs/statistics/detail/distribution_toolkit/test/function_ptrs.cpp>

int main(){

    example_distribution_function(std::cout);
    example_random(std::cout);
    test_unnormalized_functions<double>();
    test_function_ptrs<double>();

    return 0;
}