/**
 * \file boost/mirror/intrinsic/point_to.hpp
 * Returns the iterator pointing to the meta-object
 * on the specified position in a meta-object sequence.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_INTRINSIC_POINT_TO_HPP
#define BOOST_MIRROR_INTRINSIC_POINT_TO_HPP

#include <boost/mirror/intrinsic/detail/point_to.hpp>
#include <boost/mirror/intrinsic/detail/positional.hpp>

namespace boost {
namespace mirror {
namespace detail {

	template <class MetaObjectSequence, class Position>
	struct point_to_impl
	: explicit_position_getter_impl<
		MetaObjectSequence, 
		Position,
		meta_object_sequence_point_to
	>
	{ };

} // namespace detail

template <class MetaObjectSequence, class Position>
struct point_to : detail::point_to_impl<MetaObjectSequence, Position>
{ };

} // namespace mirror
} // namespace boost

#endif //include guard

