/**
 * \file boost/mirror/detail/static_log10.hpp
 * Template that allows to calculate the "integral decadic
 * logarithm" from the given compile-time integral constant.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_DETAIL_STATIC_LOG10_HPP
#define BOOST_MIRROR_META_DETAIL_STATIC_LOG10_HPP

#include <boost/mpl/vector.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/comparison.hpp>

namespace boost {
namespace mirror {
namespace detail {

typedef mpl::vector<
	mpl::pair<mpl::int_<1000000000>, mpl::int_<9> >,
	mpl::pair<mpl::int_< 100000000>, mpl::int_<8> >,
	mpl::pair<mpl::int_<  10000000>, mpl::int_<7> >,
	mpl::pair<mpl::int_<   1000000>, mpl::int_<6> >,
	mpl::pair<mpl::int_<    100000>, mpl::int_<5> >,
	mpl::pair<mpl::int_<     10000>, mpl::int_<4> >,
	mpl::pair<mpl::int_<      1000>, mpl::int_<3> >, 
	mpl::pair<mpl::int_<       100>, mpl::int_<2> >, 
	mpl::pair<mpl::int_<        10>, mpl::int_<1> >, 
	mpl::pair<mpl::int_<         1>, mpl::int_<0> >
> static_log10_boundaries;


template <int I>
struct static_log10
{
	typedef typename mpl::find_if<
		static_log10_boundaries, 
		mpl::less_equal<
			mpl::first<mpl::_1>,
			mpl::int_<I>
		>
	>::type position;


	typedef typename mpl::second<
			typename mpl::deref<
				position
		>::type 
	>::type type;
};

} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

