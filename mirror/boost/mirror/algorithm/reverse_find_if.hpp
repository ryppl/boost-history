/**
 * \file boost/mirror/algorithm/reverse_find_if.hpp
 *
 * Returns the reverse iterator pointing to the first-from-end meta-object
 * in a meta-object sequence satisfying the given predicate.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ALGORITHM_REVERSE_FIND_IF_HPP
#define BOOST_MIRROR_ALGORITHM_REVERSE_FIND_IF_HPP

#include <boost/mirror/algorithm/detail/reverse_find_if.hpp>
#include <boost/mirror/intrinsic/detail/positional.hpp>

namespace boost {
namespace mirror {
namespace detail {

	template <class MetaObjectSequence, class Predicate>
	struct reverse_find_if_impl 
	: position_getter_impl_1<MetaObjectSequence, Predicate, meta_object_reverse_find_if>
	{ };

} // namespace detail

template <class MetaObjectSequence, class Predicate>
struct reverse_find_if : detail::reverse_find_if_impl<MetaObjectSequence, Predicate>
{ };

} // namespace mirror
} // namespace boost

#endif //include guard

