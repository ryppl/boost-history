//  Copyright (c) 2006, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TREE_SORTABLE_TRAITS_HPP
#define BOOST_TREE_SORTABLE_TRAITS_HPP


namespace boost {
namespace tree {

template <class Sortable>
struct sortable_traits
{
	typedef Sortable sortable_type;
	typedef typename sortable_type::iterator cursor;
	typedef typename sortable_type::value_type value_type;
 	typedef typename sortable_type::size_type size_type;
 	typedef sortable_type container_type; //remove const
 	//typedef sortable_type const_container_type; //remove const
};

//template <class Range>
//struct head_wrapper {
//	typedef Range sortable_type;
//	typename sortable_traits<Range>::container_type operator()(Range const& c) // TODO: const_ //use sortable_traits for return type?
//	{
//		return c;
//	}
//};

template <class Range>
bool empty(Range)
{
	return true;
}


template <class Range>
bool empty(typename Range::iterator)
{
	return false;
}


template <class Range>
bool empty(typename Range::const_iterator)
{
	return false;
}

template <class Range>
typename sortable_traits<Range>::container_type head(Range const& c) // TODO: const_ //use sortable_traits for return type?
{
	return c;
}


} // namespace tree
} // namespace boost


#endif // BOOST_TREE_SORTABLE_TRAITS_HPP
