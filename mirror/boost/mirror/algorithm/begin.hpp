/**
 * \file boost/mirror/algorithm/begin.hpp
 * Returns the iterator pointing to the first meta-object
 * in a meta-object sequence.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ALGORITHM_BEGIN_HPP
#define BOOST_MIRROR_ALGORITHM_BEGIN_HPP

#include <boost/mirror/algorithm/detail/begin.hpp>
#include <boost/mirror/algorithm/detail/positional.hpp>

namespace boost {
namespace mirror {
namespace detail {

	template <class MetaObjectSequence>
	struct begin_impl
	: position_getter_impl<MetaObjectSequence, meta_object_sequence_begin>
	{ };

} // namespace detail

template <class MetaObjectSequence>
struct begin : detail::begin_impl<MetaObjectSequence>
{ };

} // namespace mirror
} // namespace boost

#endif //include guard

