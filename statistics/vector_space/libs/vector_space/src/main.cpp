///////////////////////////////////////////////////////////////////////////////
// vector_space::src::main.cpp                                               //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <libs/vector_space/example/difference.h>
#include <libs/vector_space/example/equal.h>

#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/casts.hpp>

#include <boost/typeof/typeof.hpp>

    using namespace boost;



int main(){

    example_difference(std::cout);
    example_equal(std::cout);
    

    return 0;
}
