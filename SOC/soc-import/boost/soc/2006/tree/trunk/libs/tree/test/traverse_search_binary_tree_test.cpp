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
#include <boost/tree/searcher.hpp>

#include <boost/tree/traversal.hpp>

#include <vector>
#include <algorithm>

#include <boost/test/minimal.hpp>

//TODO: Make this a test suite.

//does boost have timers? what does the austern et al one look like?
// TODO: get timings. that makes that no testcase anymore, right?

// Test data from http://en.wikipedia.org/wiki/Image:Binary_search_tree.svg
// (with root modified to carry 8 instead of 7, and with two additional nodes:
// 11 inserted left of 13; 12 right of 11)
// and in combination with http://en.wikipedia.org/wiki/Tree_traversal#Examples
// (as tree shapes are equal [apart from the extra nodes])

using boost::tree::searcher;
using boost::tree::binary_tree;
typedef searcher<false, binary_tree<int> > searcher_t;


binary_tree<int> test_data_tree()
{
	binary_tree<int> ret;
	binary_tree<int>::cursor cur = ret.insert(ret.shoot(), 8);
	cur = ret.insert(cur, 3);
	ret.insert(cur, 1);
	cur = ret.insert(++cur, 6);
	ret.insert(cur, 4);
	ret.insert(++cur, 7);	
	cur = ret.insert(ret.root().end(), 10);
	cur = ret.insert(ret.root().end().end(), 14);	
	cur = ret.insert(cur, 13);
	cur = ret.insert(cur, 11);
	cur = ret.insert(++cur, 12);

	BOOST_CHECK(*(ret.root().begin()) == 8);
	BOOST_CHECK(*(ret.root().begin().begin()) == 3);
	BOOST_CHECK(*(ret.root().begin().begin().begin()) == 1);
	BOOST_CHECK(*(ret.root().begin().end().begin()) == 6);		
	BOOST_CHECK(*(ret.root().begin().end().begin().begin()) == 4);	
	BOOST_CHECK(*(ret.root().begin().end().end().begin()) == 7);	

	BOOST_CHECK(*(ret.root().end().begin()) == 10);
	BOOST_CHECK(*(ret.root().end().end().begin()) == 14);
	BOOST_CHECK(*(ret.root().end().end().begin().begin()) == 13);
	BOOST_CHECK(*(ret.root().end().end().begin().begin().begin()) == 11);
	BOOST_CHECK(*(ret.root().end().end().begin().begin().end().begin()) == 12);
	
	return ret;
}

searcher_t test_data_searcher()
{
	searcher_t my_tree;
	
	my_tree.insert(8);
	my_tree.insert(10);
	my_tree.insert(14);
	my_tree.insert(13);
	my_tree.insert(11);
	my_tree.insert(12);

	my_tree.insert(3);
	my_tree.insert(1);
	my_tree.insert(6);
	my_tree.insert(4);
	my_tree.insert(7);
	
	return my_tree;
}

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
	binary_tree<int>::const_cursor c = begin(the_tree);

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
	BOOST_CHECK(c == end(the_tree));
}

void test_inorder_iterator_traversal(searcher_t const& my_tree)
{		
	searcher_t::const_iterator ci = my_tree.begin(); 
	BOOST_CHECK(*ci++ == 1);
	BOOST_CHECK(*ci++ == 3);
	BOOST_CHECK(*ci++ == 4);
	BOOST_CHECK(*ci++ == 6);
	BOOST_CHECK(*ci++ == 7);
	BOOST_CHECK(*ci++ == 8);
	BOOST_CHECK(*ci++ == 10);
	BOOST_CHECK(*ci++ == 11);
	BOOST_CHECK(*ci++ == 12);
	BOOST_CHECK(*ci++ == 13);
	BOOST_CHECK(*ci++ == 14);
	BOOST_CHECK(ci == my_tree.end());
}

void test_reverse_inorder_iterator_traversal(searcher_t const& my_tree)
{	
	searcher_t::const_iterator ci = my_tree.end(); 
	BOOST_CHECK(*--ci == 14);
	BOOST_CHECK(*--ci == 13);
	BOOST_CHECK(*--ci == 12);
	BOOST_CHECK(*--ci == 11);
	BOOST_CHECK(*--ci == 10);
	BOOST_CHECK(*--ci == 8);
	BOOST_CHECK(*--ci == 7);
	BOOST_CHECK(*--ci == 6);
	BOOST_CHECK(*--ci == 4);
	BOOST_CHECK(*--ci == 3);
	BOOST_CHECK(*--ci == 1);
	BOOST_CHECK(ci == my_tree.begin());
}

void test_preorder_traversal(binary_tree<int> const& the_tree)
{	
	using namespace boost::tree::preorder;

	binary_tree<int>::const_cursor ci = begin(the_tree); 
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
	BOOST_CHECK(ci == end(the_tree));
}

void test_reverse_preorder_traversal(binary_tree<int> const& the_tree)
{	
	using namespace boost::tree::preorder;

	binary_tree<int>::const_cursor ci = end(the_tree); 
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
	BOOST_CHECK(ci == begin(the_tree));
}

void test_postorder_traversal(binary_tree<int> const& the_tree)
{	
	using namespace boost::tree::postorder;

	binary_tree<int>::const_cursor ci = begin(the_tree); 
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
	BOOST_CHECK(ci == end(the_tree));
}

void test_reverse_postorder_traversal(binary_tree<int> const& the_tree)
{	
	using namespace boost::tree::postorder;

	binary_tree<int>::const_cursor ci = end(the_tree); 
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
	BOOST_CHECK(ci == begin(the_tree));
}

int test_main(int, char* [])
{
	searcher_t test_searcher = test_data_searcher();
	binary_tree<int> test_tree = test_data_tree();
	
	test_inorder_traversal(test_tree);
	
	test_inorder_iterator_traversal(test_searcher);	
	test_reverse_inorder_iterator_traversal(test_searcher);
	
	test_preorder_traversal(test_tree);
	test_reverse_preorder_traversal(test_tree);
	
	test_postorder_traversal(test_tree);
	test_reverse_postorder_traversal(test_tree);
	return 0;
}


