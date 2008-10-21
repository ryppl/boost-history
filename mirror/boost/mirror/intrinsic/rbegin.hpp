/**
 * \file boost/mirror/intrinsic/rbegin.hpp
 * Returns the revverse iterator pointing to the last meta-object
 * in a meta-object sequence.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_INTRINSIC_RBEGIN_HPP
#define BOOST_MIRROR_INTRINSIC_RBEGIN_HPP

#include <boost/mirror/intrinsic/detail/rbegin.hpp>
#include <boost/mirror/intrinsic/detail/positional.hpp>

namespace boost {
namespace mirror {
namespace detail {

	template <class MetaObjectSequence>
	struct rbegin_impl
	: position_getter_impl<MetaObjectSequence, meta_object_sequence_rbegin>
	{ };

} // namespace detail

template <class MetaObjectSequence>
struct rbegin : detail::rbegin_impl<MetaObjectSequence>
{ };

} // namespace mirror
} // namespace boost

#endif //include guard

