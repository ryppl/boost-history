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

#include <boost/multi_index/identity.hpp>

using boost::tree::binary_tree;
using boost::tree::searcher;

using boost::multi_index::identity;

/**
 * @brief	test_searcher (exposes underlying container for test purposes)
 */
template <bool Multi, class Sortable, class Extract = 
		  boost::multi_index::identity<typename Sortable::value_type>,
		  class Compare = std::less<typename Extract::result_type> >
struct test_searcher
 : public searcher<Multi, Sortable, Extract, Compare> {
	Sortable& container()
	{
		return this->c;
	}
};

// Test data from http://en.wikipedia.org/wiki/Image:Binary_search_tree.svg
// (with root modified to carry 8 instead of 7, and with two additional nodes:
// 11 inserted left of 13; 12 right of 11)
// and in combination with http://en.wikipedia.org/wiki/Tree_traversal#Examples
// (as tree shapes are equal [apart from the extra nodes])
template <class Tree>
void test_data_tree(Tree& ret)
{
	typename Tree::cursor cur = ret.insert(ret.shoot(), 8);
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

//	BOOST_CHECK(*(ret.root().begin()) == 8);
//	BOOST_CHECK(*(ret.root().begin().begin()) == 3);
//	BOOST_CHECK(*(ret.root().begin().begin().begin()) == 1);
//	BOOST_CHECK(*(ret.root().begin().end().begin()) == 6);		
//	BOOST_CHECK(*(ret.root().begin().end().begin().begin()) == 4);	
//	BOOST_CHECK(*(ret.root().begin().end().end().begin()) == 7);	
//
//	BOOST_CHECK(*(ret.root().end().begin()) == 10);
//	BOOST_CHECK(*(ret.root().end().end().begin()) == 14);
//	BOOST_CHECK(*(ret.root().end().end().begin().begin()) == 13);
//	BOOST_CHECK(*(ret.root().end().end().begin().begin().begin()) == 11);
//	BOOST_CHECK(*(ret.root().end().end().begin().begin().end().begin()) == 12);
	
}


template <class Searcher>
void test_data_searcher(Searcher& my_tree)
{
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
}

template <class Searcher>
void test_inorder_iterator_traversal(Searcher const& my_tree)
{		
	typename Searcher::const_iterator ci = my_tree.begin(); 
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

template <class Searcher>
void test_reverse_inorder_iterator_traversal(Searcher const& my_tree)
{	
	typename Searcher::const_iterator ci = my_tree.end(); 
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
