//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef LIBS_TREE_TEST_HELPERS_HPP
#define LIBS_TREE_TEST_HELPERS_HPP

#include <boost/tree/binary_tree.hpp>
#include <boost/tree/balanced_tree.hpp>
#include <boost/tree/searcher.hpp>

#include <boost/multi_index/identity.hpp>

using namespace boost::tree;

using boost::multi_index::identity;


/**
 * @brief	test_balancer (exposes underlying hierarchy for test purposes)
 */
// TODO: Ctor, dtors
template <class Hier, class Balance>
struct test_balancer
 : public balanced_tree<Hier, Balance> {
 	
 	typedef typename balanced_tree<Hier, Balance>::hierarchy_type hierarchy_type;
 	
	explicit test_balancer(hierarchy_type const& hier = hierarchy_type())
	: balanced_tree<Hier, Balance>(hier)
	{}
	
	hierarchy_type& hierarchy()
	{
		return this->h;
	}
};

/**
 * @brief	test_searcher (exposes underlying container for test purposes)
 */
// TODO: Ctor, dtors
template <bool Multi, class Sortable, class Extract = 
		  boost::multi_index::identity<typename Sortable::value_type>,
		  class Compare = std::less<typename Extract::result_type> >
struct test_searcher
 : public searcher<Multi, Sortable, Extract, Compare> {
//	Sortable& 
	typename Sortable::hierarchy_type::template rebind<typename Sortable::value_type>::other&

	container()
	{
		return this->c;
	}
};


template <class Searcher>
void create_test_data_searcher(Searcher& my_tree)
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
void create_test_data_sequence(Searcher& my_tree)
{
	my_tree.push_back(8);
	my_tree.push_back(10);
	my_tree.push_back(14);
	my_tree.push_back(13);
	my_tree.push_back(11);
	my_tree.push_back(12);

	my_tree.push_back(3);
	my_tree.push_back(1);
	my_tree.push_back(6);
	my_tree.push_back(4);
	my_tree.push_back(7);
}

#endif // LIBS_TREE_TEST_HELPERS_HPP
