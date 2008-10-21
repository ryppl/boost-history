/**
 * \file boost/mirror/algorithm/detail/find_if.hpp
 *
 * Returns iterator to the first element of a meta-object
 * sequence satisfying the predicate.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ALGORITHM_DETAIL_FIND_IF_HPP
#define BOOST_MIRROR_ALGORITHM_DETAIL_FIND_IF_HPP

#include <boost/mirror/iterator/detail/iterator.hpp>

namespace boost {
namespace mirror {
namespace detail {

	template <
		class ReflectedType, 
		class VariantTag,
		class MetaObjectSequence,
		class Predicate
	>
	struct meta_object_find_if
	{
		typedef typename get_meta_object_iterator<
			ReflectedType, 
			VariantTag,
			MetaObjectSequence,
			mpl::int_<0>,
			Predicate
		>::type type;
	};

} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

