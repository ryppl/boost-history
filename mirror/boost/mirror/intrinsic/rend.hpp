/**
 * \file boost/mirror/intrinsic/rend.hpp
 * Returns the reverse iterator pointing before the first meta-object
 * in a meta-object sequence.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_INTRINSIC_REND_HPP
#define BOOST_MIRROR_INTRINSIC_REND_HPP

#include <boost/mirror/intrinsic/detail/rend.hpp>
#include <boost/mirror/intrinsic/detail/positional.hpp>

namespace boost {
namespace mirror {
namespace detail {

	template <class MetaObjectSequence>
	struct rend_impl
	: position_getter_impl<MetaObjectSequence, meta_object_sequence_rend>
	{ };

} // namespace detail

template <class MetaObjectSequence>
struct rend : detail::rend_impl<MetaObjectSequence>
{ };

} // namespace mirror
} // namespace boost

#endif //include guard

