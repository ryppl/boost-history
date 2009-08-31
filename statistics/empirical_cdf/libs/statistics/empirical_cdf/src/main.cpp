///////////////////////////////////////////////////////////////////////////////
// empirical_cdf::example::main.cpp                                          //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
#include <iostream>
#include <libs/statistics/empirical_cdf/example/proportion_less_than.h>

int main(){

    example_algorithm_proportion_less_than(std::cout);

    // See sandbox/dist_random for other examples.

    return 0;
}