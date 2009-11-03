///////////////////////////////////////////////////////////////////////////////
// statistics::detail::fusion::main.cpp                                      //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
#include <iostream>
#include <libs/statistics/detail/fusion/example/at_key.h>
#include <boost/statistics/detail/fusion/joint_view/bind.hpp>

int main(){

    example_at_key_iterator(std::cout);

    return 0;
}
