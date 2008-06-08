//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/algorithm.hpp>
#include <boost/tree/binary_tree.hpp>

#include <boost/tree/ascending_cursor.hpp>

#include <list>
#include <algorithm>
#include <iterator>

#include "helpers.hpp"
#include "test_tree_traversal_data.hpp"

using namespace boost::tree;

namespace test {
namespace ORDER {

template <class Cursor, class OutCursor, class Container>
void test_copy(Cursor c, OutCursor& o, Container& cont)
{	
	boost::tree::ORDER::copy(c.begin(), o);
	test::ORDER::traversal(cont.begin(), cont.end());
}

template <class Cursor, class OutCursor, class Container>
void test_transform(Cursor c, Cursor d, OutCursor& o, Container& cont)
{
	// First copy test_tree to test_tree2, by adding 1 to each element,
	// then copy test_tree2 to test_list, by subtracting 1 - so 
	// test_list should hold test_tree's original elements in ORDER.
	boost::tree::ORDER::transform(c.begin(), d.begin(), 
		std::bind2nd(std::plus<int>(),1));
	boost::tree::ORDER::transform(d.begin(), o,
		std::bind2nd(std::minus<int>(),1));
	test::ORDER::traversal(cont.begin(), cont.end());
}

template <class Cursor>
void algorithms(Cursor c, Cursor d)
{
	std::list<int> test_list;
	typedef std::back_insert_iterator< std::list<int> > back_insert_iter_list_int;
	typedef output_cursor_iterator_wrapper<back_insert_iter_list_int> oc_bi_lst_type;
	back_insert_iter_list_int it_test_list = std::back_inserter(test_list);
	oc_bi_lst_type oc_test_list = oc_bi_lst_type(it_test_list);
	
	test_copy(c, oc_test_list, test_list);
	
	test_list.clear();
	test_transform(c, d, oc_test_list, test_list);
}

void compare_cursor_to_iterator_traversal(boost::tree::binary_tree<int> const& t) {
	
	using boost::tree::ascending_cursor;
	
	typedef boost::tree::binary_tree<int>::const_cursor cursor;
	
	std::list<int> test_list;
	typedef std::back_insert_iterator< std::list<int> > back_insert_iter_list_int;
	typedef output_cursor_iterator_wrapper<back_insert_iter_list_int> oc_bi_lst_type;
	back_insert_iter_list_int it_test_list = std::back_inserter(test_list);
	oc_bi_lst_type oc_test_list = oc_bi_lst_type(it_test_list);
	
	boost::tree::ORDER::copy(t.croot().begin(), oc_test_list);

	// Are the elements accessed in the correct order?
	BOOST_CHECK(std::equal(	boost::tree::ORDER::begin(t.root()),
							boost::tree::ORDER::end(t.root()),
							test_list.begin()
							));

	// Does end() mark the right element? 
	BOOST_CHECK(std::distance(boost::tree::ORDER::begin(t.root()),
							  boost::tree::ORDER::end(t.root())) == 
				std::distance(test_list.begin(), test_list.end()));

	// Reverse order.
	BOOST_CHECK(std::equal(	boost::tree::ORDER::rbegin(t.root()),
							boost::tree::ORDER::rend(t.root()),
							test_list.rbegin()
							));

	BOOST_CHECK(std::distance(boost::tree::ORDER::rbegin(t.root()),
							  boost::tree::ORDER::rend(t.root())) == 
				std::distance(test_list.rbegin(), test_list.rend()));					

	//Now same for iterators wrapped around "explicit stack"-based cursors
	BOOST_CHECK(std::equal(	boost::tree::ORDER::begin(ascending_cursor<cursor>(t.root())),
							boost::tree::ORDER::end(ascending_cursor<cursor>(t.root())),
							test_list.begin()
							));

	BOOST_CHECK(std::distance(boost::tree::ORDER::begin(ascending_cursor<cursor>(t.root())),
							  boost::tree::ORDER::end(ascending_cursor<cursor>(t.root()))) == 
				std::distance(test_list.begin(), test_list.end()));

	BOOST_CHECK(std::equal(	boost::tree::ORDER::rbegin(ascending_cursor<cursor>(t.root())),
							boost::tree::ORDER::rend(ascending_cursor<cursor>(t.root())),
							test_list.rbegin()
							));
							
	BOOST_CHECK(std::distance(boost::tree::ORDER::rbegin(ascending_cursor<cursor>(t.root())),
							  boost::tree::ORDER::rend(ascending_cursor<cursor>(t.root()))) == 
				std::distance(test_list.rbegin(), test_list.rend()));

}


} // namespace ORDER
} // namespace test
