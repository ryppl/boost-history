///////////////////////////////////////////////////////////////////////////////
// statistics::kernel::src::main.cpp                                         //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
#include <iostream>
#include <libs/statistics/kernel/example/scalar_rp.h>
#include <libs/statistics/kernel/example/scalar_nw.h>
#include <libs/statistics/kernel/example/kernel_mono_rp.h>
#include <libs/statistics/kernel/example/benchmark_scalar.h>


int main(){

    example_scalar_rp(std::cout);
    example_scalar_nw(std::cout);
    example_kernel_mono_rp(std::cout);
    //example_benchmark_scalar(std::cout);
    
    return 0;
}