//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** 
 * @file search.hpp
 * Generalized binary (tree) search algorithms
 */
 
// TODO: 
// * Concept checks all over.
// * add versions without cmp argument (just using <)

// concept checks etc. ATM binary only, generalize for actual Multiway.
// 2/3 argument versions (using tree instead of root/shoot)
//(and specialize again using "trivial lower bound" instead of std::lower_bound)
//internally - or provide one lower bound for all. (for example a range version)
// (See "exercises"!)

#ifndef BOOST_TREE_SEARCH_HPP
#define BOOST_TREE_SEARCH_HPP

#include <algorithm>

namespace boost {
namespace tree {


/**
 * @brief		Finds the first position in a multiway subtree in which @a val 
 * 				could be inserted without changing the ordering, using < (less
 * 				than) 
 * 				for comparisons.
 * @param x		The subtree's root
 * @param y 		The subtree's shoot
 * @param val	The search term
 * @return		A multiway cursor pointing to the first element not less than 
 *				@a val, or @y if every element in the subtree is less than 
 * 				@a val.
 */
template <class MultiwayCursor, class T>
MultiwayCursor lower_bound(MultiwayCursor x, T const& val)
{
	MultiwayCursor y = x;
	while (!x.empty()) {
		x = std::lower_bound(x.begin(), x.end(), val);
	if (x.parity() == 0)
			y = x;
	}
	return y;
}

/**
 * @brief		Finds the first position in a multiway subtree in which @a val 
 * 				could be inserted without changing the ordering, using @a cmp 
 * 				for comparisons.
 * @param x		The subtree's root
 * @param y 		The subtree's shoot
 * @param val	The search term
 * @param cmp	The comparison functor
 * @return		A multiway cursor pointing to the first element not less than 
 *				@a val, or @y if every element in the subtree is less than 
 * 				@a val.
 */
template <class MultiwayCursor, class T, class Cmp>
MultiwayCursor lower_bound(MultiwayCursor x, T const& val, Cmp cmp)
{
	MultiwayCursor y = x;
	while (!x.empty()) {
		x = std::lower_bound(x.begin(), x.end(), val, cmp);
	if (x.parity() == 0)
			y = x;
	}
	return y;
}


} // namespace tree
} // namespace boost

#endif // BOOST_TREE_SEARCH_HPP


