//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/binary_tree.hpp>
//[ foreach_include_algorithm
#include <boost/tree/algorithm.hpp>
//]

#include <iostream>

#include "../test/test_tree_traversal_data.hpp"

using namespace boost::tree;

//[ for_each
void to_cout(int i) {
    std::cout << ' ' << i;
    return;
}

int main() {
    binary_tree<int> bt;
    
    // Fill it with data...
    create_test_data_tree(bt);

    std::cout << "Preorder:";
    preorder::for_each(bt.root(), to_cout);
    
    std::cout << std::endl << "Inorder:";
    inorder::for_each(bt.root(), to_cout);
    
    std::cout << std::endl << "Postorder:";
    postorder::for_each(bt.root(), to_cout);

    return 0;
}
//]