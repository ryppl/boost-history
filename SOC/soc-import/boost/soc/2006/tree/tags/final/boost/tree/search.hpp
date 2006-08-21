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
MultiwayCursor lower_bound(MultiwayCursor x, MultiwayCursor y, T const& val)
{
	while (x.empty()) {
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
MultiwayCursor lower_bound(MultiwayCursor x, MultiwayCursor y, T const& val, 
						   Cmp cmp)
{
	while (x.empty()) {
		x = std::lower_bound(x.begin(), x.end(), val, cmp);
		if (x.parity() == 0)
			y = x;
	}
	return y;
}


} // namespace tree
} // namespace boost

#endif // BOOST_TREE_SEARCH_HPP


