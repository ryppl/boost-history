///////////////////////////////////////////////////////////////////////////////
// statistics::survival::model::src::main.cpp                                //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <libs/statistics/survival/model/example/exponential.h>
#include <libs/statistics/survival/model/example/posterior_analysis.h>

int main(){

    example_exponential(std::cout);
    example_posterior_analysis(std::cout);
    
    return 0;
}