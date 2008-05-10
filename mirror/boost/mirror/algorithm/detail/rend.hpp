/**
 * \file boost/mirror/algorithm/detail/rend.hpp
 * Template returning the iterator poining before 
 * the first element in a meta object sequence.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ALGORITHM_DETAIL_REND_HPP
#define BOOST_MIRROR_ALGORITHM_DETAIL_REND_HPP

#include <boost/mirror/algorithm/detail/iterator.hpp>
#include <boost/mpl/always.hpp>

namespace boost {
namespace mirror {
namespace detail {

	template <
		class ReflectedType, 
		class VariantTag,
		class MetaObjectSequence,
		class Dummy
	>
	struct meta_object_sequence_rend
	{
		typedef typename get_meta_object_reverse_iterator<
			ReflectedType, 
			VariantTag,
			MetaObjectSequence,
			mpl::int_<-1>,
			mpl::always<mpl::true_>
		>::type type;
	};


} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

