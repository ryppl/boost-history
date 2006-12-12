//  Copyright (c) 2006, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

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
// Test data from http://en.wikipedia.org/wiki/Image:Binary_search_tree.svg
// (with root modified to carry 8 instead of 7, and with two additional nodes:
// 11 inserted left of 13; 12 right of 11)
// and in combination with http://en.wikipedia.org/wiki/Tree_traversal#Examples
// (as tree shapes are equal [apart from the extra nodes])
template <class Tree>
void create_test_data_tree(Tree& ret)
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
}

template <class Tree>
void validate_test_data_tree(Tree const& ret)
{
	BOOST_CHECK(*ret.root().begin() == 8);
	BOOST_CHECK(*ret.root().begin().begin() == 3);	
	BOOST_CHECK(*ret.root().begin().begin().begin() == 1);
	BOOST_CHECK(*ret.root().begin().end().begin() == 6);		
	BOOST_CHECK(*ret.root().begin().end().begin().begin() == 4);	
	BOOST_CHECK(*ret.root().begin().end().end().begin() == 7);	

	BOOST_CHECK(*ret.root().end().begin() == 10);
	BOOST_CHECK(*ret.root().end().end().begin() == 14);
	BOOST_CHECK(*ret.root().end().end().begin().begin() == 13);
	BOOST_CHECK(*ret.root().end().end().begin().begin().begin() == 11);
	BOOST_CHECK(*ret.root().end().end().begin().begin().end().begin() == 12);
}

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


