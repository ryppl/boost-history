/**
 * \file boost/mirror/intrinsic/detail/end.hpp
 * Template returning the iterator poining to 
 * the first element in a meta object sequence.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_INTRINSIC_DETAIL_END_HPP
#define BOOST_MIRROR_INTRINSIC_DETAIL_END_HPP

#include <boost/mirror/iterator/detail/iterator.hpp>
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
	struct meta_object_sequence_end
	{
		typedef typename get_meta_object_iterator<
			ReflectedType, 
			VariantTag,
			MetaObjectSequence,
			mpl::int_<size<MetaObjectSequence>::value>,
			mpl::always<mpl::true_>
		>::type type;
	};


} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

