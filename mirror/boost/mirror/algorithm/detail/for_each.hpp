/**
 * \file boost/mirror/algorithm/detail/for_each.hpp
 * Implementation of the for_each algorithm 
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ALGORITHM_DETAIL_FOR_EACH_HPP
#define BOOST_MIRROR_ALGORITHM_DETAIL_FOR_EACH_HPP

#include <boost/mirror/algorithm/detail/iterative.hpp>
#include <boost/mirror/iterator/next.hpp>

namespace boost {
namespace mirror {
namespace detail {

	/** Implementation of the for_each function
	*/
	template <class IteratorBegin, class IteratorEnd>
	struct for_each_meta_object
	: perform_on_range<
		IteratorBegin,
		IteratorEnd,
		mpl::int_< 1 >
	>{ };

} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

