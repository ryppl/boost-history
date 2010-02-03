///////////////////////////////////////////////////////////////////////////////
// statistics::detail::fusion::main.cpp                                      //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
#include <iostream>
#include <libs/statistics/detail/fusion/example/at_key.h>
#include <libs/statistics/detail/fusion/example/serialize.h>
#include <libs/statistics/detail/fusion/example/joint_view_binder.h>
#include <libs/statistics/detail/fusion/example/joint_view_flatten_bind_range.h>

int main(){

    example_at_key_iterator(std::cout);
    example_serialize(std::cout);
    example_joint_view_binder(std::cout);
    example_joint_view_flatten_bind_range(std::cout);

    return 0;
}
