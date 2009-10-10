///////////////////////////////////////////////////////////////////////////////
// kernel::src::main.cpp                                                     //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
#include <iostream>
#include <libs/statistics/detail/kernel/example/scalar_rp.h>
#include <libs/statistics/detail/kernel/example/scalar_nw.h>
#include <libs/statistics/detail/kernel/example/mv_mono_bw_rp.h>
#include <libs/statistics/detail/kernel/test/scalar_nw.h>


int main(){

    example_scalar_rp(std::cout);
    example_scalar_nw(std::cout);
    example_mv_mono_bw_rp(std::cout);
    test_scalar_nw(std::cout);
    
    return 0;
}