/**
 * \file boost/mirror/detail/static_pow10.hpp
 * Template that allows to calculate the "integral power of 10"
 * from the given compile-time integral constant.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_DETAIL_STATIC_POW10_HPP
#define BOOST_MIRROR_META_DETAIL_STATIC_POW10_HPP

#include <boost/mirror/detail/static_log10.hpp>

namespace boost {
namespace mirror {
namespace detail {

template <int I>
struct static_pow10
{
	typedef typename mpl::find_if<
		static_log10_boundaries, 
		mpl::equal_to<
			mpl::second<mpl::_1>,
			mpl::int_<I>
		>
	>::type position;

	typedef typename mpl::first<
			typename mpl::deref<
				position
		>::type 
	>::type type;

};

} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

