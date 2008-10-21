/**
 * \file boost/mirror/algorithm/detail/reverse_find_if.hpp
 *
 * Returns iterator to the first-from-end element of a meta-object
 * sequence satisfying the predicate.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ALGORITHM_DETAIL_REVERSE_FIND_IF_HPP
#define BOOST_MIRROR_ALGORITHM_DETAIL_REVERSE_FIND_IF_HPP

#include <boost/mirror/iterator/detail/reverse_iterator.hpp>

namespace boost {
namespace mirror {
namespace detail {

	template <
		class ReflectedType, 
		class VariantTag,
		class MetaObjectSequence,
		class Predicate
	>
	struct meta_object_reverse_find_if
	{
		typedef typename get_meta_object_reverse_iterator<
			ReflectedType, 
			VariantTag,
			MetaObjectSequence,
			mpl::int_<size<MetaObjectSequence>::value - 1>,
			Predicate
		>::type type;
	};

} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

