///////////////////////////////////////////////////////////////////////////////
// distribution::joint::src::main.cpp                                        //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <libs/statistics/detail/distribution/joint/test/functions.cpp>
#include <libs/statistics/detail/distribution/joint/example/random.h>

int main(){
    typedef double val_;
    typedef std::vector<val_> vec_;

    example_random(std::cout);
    test_functions<val_,vec_>(std::cout);

    return 0;
}
