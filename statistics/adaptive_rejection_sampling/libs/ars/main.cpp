///////////////////////////////////////////////////////////////////////////////
// ars::main.cpp                                                             //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <libs/ars/example/search_reflection.h>
#include <libs/ars/example/standard_distribution.h>

int main(){

    example_search_reflection(std::cout);
    example_standard_distribution(std::cout);

    return 0;

}
