///////////////////////////////////////////////////////////////////////////////
// importance_sampling::src::main.cpp                                        //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <boost/assert.hpp>
#include <libs/statistics/detail/importance_sampling/example/sampler.h>
#include <libs/statistics/detail/importance_sampling/example/scale_to_finite_sum.h>

int main(){
    
    example_sampler(std::cout);
    example_scale_to_finite_sum(std::cout);
    
    return 0;
}

