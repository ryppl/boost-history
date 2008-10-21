/**
 * \file boost/mirror/intrinsic/detail/point_to.hpp
 *
 * Template returning the iterator poining to 
 * the element at the given position
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_INTRINSIC_DETAIL_POINT_TO_HPP
#define BOOST_MIRROR_INTRINSIC_DETAIL_POINT_TO_HPP

#include <boost/mirror/iterator/detail/iterator.hpp>
#include <boost/mpl/always.hpp>

namespace boost {
namespace mirror {
namespace detail {

	template <
		class ReflectedType, 
		class VariantTag,
		class MetaObjectSequence,
		class Position
	>
	struct meta_object_sequence_point_to
	{
		typedef typename get_meta_object_iterator<
			ReflectedType, 
			VariantTag,
			MetaObjectSequence,
			Position,
			mpl::always<mpl::true_>
		>::type type;
	};

} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

