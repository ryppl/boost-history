/**
 * \file boost/mirror/detail/static_int_to_str.hpp
 * 
 * Templates that allow to query the required length of a string
 * that should hold a given decadic compile time integral value
 * and conversion of compile-time integers to strings.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_DETAIL_STATIC_INT_TO_STR_HPP
#define BOOST_MIRROR_META_DETAIL_STATIC_INT_TO_STR_HPP

#include <assert.h>
#include <boost/mirror/detail/static_log10.hpp>
#include <boost/mirror/detail/static_pow10.hpp>
#include <boost/char_type_switch/string.hpp>

namespace boost {
namespace mirror {
namespace detail {

template <int I>
struct static_int_to_str 
{
	// the length of the string needed to hold the given integer
	typedef typename mpl::next<
		typename static_log10<I>::type
	>::type length;
	//
	//
	template <int J>
	static inline bchar get_digit(mpl::int_<J> pos)
	{
		static const bchar zero = BOOST_STR_LIT('0');
		typedef typename static_pow10<
			length::value - mpl::int_<J>::value
		>::type K;
		return zero + (I / K::value) % 10;
	}
	//
	static inline void do_copy_to(bchar* _str, mpl::int_<0>){	}
	//
	template <int J>
	static inline void do_copy_to(bchar* _str, mpl::int_<J> pos)
	{
		_str[J-1] = get_digit(pos);
		do_copy_to(_str, mpl::int_<J - 1>());
	}
	//
	static bchar* convert(bchar* _str, size_t _max_len)
	{
		assert(_max_len > length::value);
		do_copy_to(_str, length());
		_str[length::value] = BOOST_STR_LIT('\0');
		return _str;
	}
};

} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

