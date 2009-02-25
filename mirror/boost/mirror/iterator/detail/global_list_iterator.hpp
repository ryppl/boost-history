/**
 * \file boost/mirror/iterator/detail/global_list_iterator.hpp
 * Iterators for iterating through meta-object sequences
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ITERATOR_DETAIL_GLOBAL_LIST_ITERATOR_HPP
#define BOOST_MIRROR_ITERATOR_DETAIL_GLOBAL_LIST_ITERATOR_HPP

#include <boost/mirror/iterator/detail/common_iterator.hpp>
#include <boost/mirror/detail/global_list.hpp>
#include <boost/mirror/intrinsic/at.hpp>


namespace boost {
namespace mirror {
namespace detail {

	/** Iterator for iterating through the items
	 *  in a global_list
	 */
	template <
		class Selector, 
		class Counter,
		class Filter
	>
	struct iterator_pointee_selector<
		counter::filtered_global_list<Selector, Counter, Filter>
	>
	{
		template <
			class GlobalList,
			class ItemPos,
			class UnaryPredicate
		>
		struct apply
		{
			typedef typename at<
				GlobalList,
				ItemPos
			>::type type;
		};
	};


} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

