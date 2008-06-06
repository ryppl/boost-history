//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/multiway_tree.hpp>

#include <boost/test/minimal.hpp>

//TODO: Make this a test suite.

void test_multiway_tree()
{
	using namespace boost::tree;
	
	typedef multiway_tree<int> tree_type;
	tree_type mytree;

	//tree_type::cursor c = 
	//mytree.insert(mytree.root(), 3);
}

int test_main(int, char* [])
{
	test_multiway_tree();
	return 0;
}
