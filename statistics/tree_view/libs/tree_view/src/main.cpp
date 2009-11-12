///////////////////////////////////////////////////////////////////////////////
// example::tree.h                                                           //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <libs/tree_view/example/tree.h>
#include <libs/tree_view/example/depth_first.h>

int main(){
    
    example_tree(std::cout);
    example_depth_first(std::cout);
    
    return 0;
}