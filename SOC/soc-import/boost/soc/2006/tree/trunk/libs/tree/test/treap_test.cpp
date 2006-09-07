//  Copyright (c) 2006, Bernhard Reiter
//
//  This code may be used under either of the following two licences:
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy 
//  of this software and associated documentation files (the "Software"), to deal 
//  in the Software without restriction, including without limitation the rights 
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
//  copies of the Software, and to permit persons to whom the Software is 
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in 
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
//  THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/binary_tree.hpp>
#include <boost/tree/balancers/treap.hpp>
#include <boost/tree/augmentors/trivial.hpp>

#include <boost/tree/searcher.hpp>

#include "test_data.hpp"

#include <boost/test/minimal.hpp>

using boost::tree::binary_tree;

//TODO: Make this a test suite.



void test_treap()
{
	using namespace boost::tree;
	
	typedef binary_tree<int, treap_balance> tree_t;
	typedef test_searcher<false, tree_t> searcher_t;
	searcher_t my_searcher;
	
	test_data_searcher(my_searcher);
	
	test_inorder_iterator_traversal(my_searcher);
	
	tree_t& my_tree = my_searcher.container();
	
	// TODO: a level-order algo? 
	
	for (tree_t::const_iterator ci = my_tree.cbegin(); ci != my_tree.cend(); ++ci)
		BOOST_CHECK(tree_t::const_cursor(ci).metadata().get_priority() 
				  < tree_t::const_cursor(ci).parent().metadata().get_priority());	
}


int test_main(int, char* [])
{
	test_treap();
	return 0;
}
