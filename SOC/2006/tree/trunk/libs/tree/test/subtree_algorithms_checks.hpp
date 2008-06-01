//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/algorithm.hpp>

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

}
}
