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

//TODO: Make this a test suite.
//		Add iterator traversal tests - check if proper overloads (if present)
//		are used.

// TODO: get timings. that makes that no testcase anymore, right?
//does boost have timers? what does the austern et al one look like?

#include <boost/tree/binary_tree.hpp>
#include <boost/tree/searcher.hpp>

#include <boost/tree/traversal.hpp>

#include <vector>
#include <algorithm>

#include <boost/test/minimal.hpp>

#include "test_data.hpp"

using boost::tree::binary_tree;
using boost::tree::searcher;
typedef test_searcher<false, binary_tree<int> > searcher_t;

//std::vector<int> preorder_data()
//{
//	std::vector ret(9);
//	ret[0] = 8;
//	ret[1] = 3;
//	ret[2] = 1;
//	ret[3] = 6;
//	ret[4] = 4;
//	ret[5] = 7;
//	ret[6] = 10;
//	ret[7] = 14;
//	ret[8] = 13;
//}

void test_inorder_traversal(binary_tree<int> const& the_tree)
{	
	using namespace boost::tree::inorder;
	binary_tree<int>::const_cursor c = first(the_tree);

	BOOST_CHECK(*c == 1);
	forward(c);
	BOOST_CHECK(*c == 3);
	forward(c);
	BOOST_CHECK(*c == 4);
	forward(c);
	BOOST_CHECK(*c == 6);
	forward(c);
	BOOST_CHECK(*c == 7);
	forward(c);
	BOOST_CHECK(*c == 8);
	forward(c);
	BOOST_CHECK(*c == 10);
	forward(c);
	BOOST_CHECK(*c == 11);
	forward(c);
	BOOST_CHECK(*c == 12);
	forward(c);
	BOOST_CHECK(*c == 13);
	forward(c);
	BOOST_CHECK(*c == 14);
	forward(c);
	BOOST_CHECK(c == last(the_tree));
}

void test_preorder_traversal(binary_tree<int> const& the_tree)
{	
	using namespace boost::tree::preorder;

	binary_tree<int>::const_cursor ci = first(the_tree); 
	BOOST_CHECK(*ci == 8);
	forward(ci);
	BOOST_CHECK(*ci == 3);
	forward(ci);
	BOOST_CHECK(*ci == 1);
	forward(ci);
	BOOST_CHECK(*ci == 6);
	forward(ci);
	BOOST_CHECK(*ci == 4);
	forward(ci);
	BOOST_CHECK(*ci == 7);
	forward(ci);
	BOOST_CHECK(*ci == 10);
	forward(ci);
	BOOST_CHECK(*ci == 14);
	forward(ci);
	BOOST_CHECK(*ci == 13);
	forward(ci);
	BOOST_CHECK(*ci == 11);
	forward(ci);
	BOOST_CHECK(*ci == 12);
	forward(ci);
	BOOST_CHECK(ci == last(the_tree));
}

void test_reverse_preorder_traversal(binary_tree<int> const& the_tree)
{	
	using namespace boost::tree::preorder;

	binary_tree<int>::const_cursor ci = last(the_tree); 
	BOOST_CHECK(*ci == 12);
	back(ci);
	BOOST_CHECK(*ci == 11);
	back(ci);
	BOOST_CHECK(*ci == 13);
	back(ci);
	BOOST_CHECK(*ci == 14);
	back(ci);
	BOOST_CHECK(*ci == 10);
	back(ci);
	BOOST_CHECK(*ci == 7);
	back(ci);
	BOOST_CHECK(*ci == 4);
	back(ci);
	BOOST_CHECK(*ci == 6);
	back(ci);
	BOOST_CHECK(*ci == 1);
	back(ci);
	BOOST_CHECK(*ci == 3);
	back(ci);
	BOOST_CHECK(*ci == 8);
	BOOST_CHECK(ci == first(the_tree));
}

void test_postorder_traversal(binary_tree<int> const& the_tree)
{	
	using namespace boost::tree::postorder;

	binary_tree<int>::const_cursor ci = first(the_tree); 
	BOOST_CHECK(*ci == 1);	
	forward(ci);
	BOOST_CHECK(*ci == 4);
	forward(ci);
	BOOST_CHECK(*ci == 7);
	forward(ci);
	BOOST_CHECK(*ci == 6);
	forward(ci);
	BOOST_CHECK(*ci == 3);
	forward(ci);
	BOOST_CHECK(*ci == 12);
	forward(ci);
	BOOST_CHECK(*ci == 11);
	forward(ci);
	BOOST_CHECK(*ci == 13);
	forward(ci);
	BOOST_CHECK(*ci == 14);
	forward(ci);
	BOOST_CHECK(*ci == 10);
	forward(ci);
	BOOST_CHECK(*ci == 8);
	forward(ci);
	BOOST_CHECK(ci == last(the_tree));
}

void test_reverse_postorder_traversal(binary_tree<int> const& the_tree)
{	
	using namespace boost::tree::postorder;

	binary_tree<int>::const_cursor ci = last(the_tree); 
	back(ci);
	BOOST_CHECK(*ci == 8);	
	back(ci);
	BOOST_CHECK(*ci == 10);
	back(ci);
	BOOST_CHECK(*ci == 14);
	back(ci);
	BOOST_CHECK(*ci == 13);
	back(ci);
	BOOST_CHECK(*ci == 11);
	back(ci);
	BOOST_CHECK(*ci == 12);
	back(ci);
	BOOST_CHECK(*ci == 3);
	back(ci);
	BOOST_CHECK(*ci == 6);
	back(ci);
	BOOST_CHECK(*ci == 7);
	back(ci);
	BOOST_CHECK(*ci == 4);
	back(ci);
	BOOST_CHECK(*ci == 1);
	BOOST_CHECK(ci == first(the_tree));
}

// orphaned == no parent() links...
// TODO: const parameter
void test_orphaned_inorder_traversal(binary_tree<int>& the_tree)
{	
	boost::tree::inorder::iterator<binary_tree<int>::cursor, boost::forward_traversal_tag>
		ci = boost::tree::inorder::begin(the_tree, boost::forward_traversal_tag());
	BOOST_CHECK(*ci == 1);
	++ci;
	BOOST_CHECK(*ci == 3);
	++ci;
	BOOST_CHECK(*ci == 4);	
	++ci;
	BOOST_CHECK(*ci == 6);
	++ci;
	BOOST_CHECK(*ci == 7);
	++ci;
	BOOST_CHECK(*ci == 8);
	++ci;
	BOOST_CHECK(*ci == 10);
	++ci;
	BOOST_CHECK(*ci == 11);
	++ci;
	BOOST_CHECK(*ci == 12);
	++ci;
	BOOST_CHECK(*ci == 13);
	++ci;
	BOOST_CHECK(*ci == 14);
	//++ci;
	//BOOST_CHECK(ci == boost::tree::inorder::end(the_tree, boost::forward_traversal_tag()));
}

int test_main(int, char* [])
{
	searcher_t test_searcher;
	test_data_searcher(test_searcher);
	binary_tree<int>& test_tree = test_searcher.container();
	//test_data_tree(test_tree);
	
	test_inorder_traversal(test_tree);
	
	test_inorder_iterator_traversal(test_searcher);	
	test_reverse_inorder_iterator_traversal(test_searcher);
	
	test_preorder_traversal(test_tree);
	test_reverse_preorder_traversal(test_tree);
	
	test_postorder_traversal(test_tree);
	test_reverse_postorder_traversal(test_tree);
	
	test_orphaned_inorder_traversal(test_tree);
	
	return 0;
}


