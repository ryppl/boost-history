///////////////////////////////////////////////////////////////////////////////
// statistics::survival::src::main.cpp                                       //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <libs/statistics/survival/data/example/data.h>
#include <libs/statistics/survival/data/example/random.h>

int main(){

    //example_data(std::cout);
    {
        example_random(
            1e2,
            5e4,
            0.0,
            5.0,
            0.0,
            0.0,
            "/Users/erwann/projets/2009/Xcode/survival/build/Release/",
            std::cout
        );
    }

    return 0;
}
