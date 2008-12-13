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

#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/plus.hpp>

namespace boost {
namespace mirror {
namespace detail {

template <typename CharT>
struct static_int_to_str_constants;

template <>
struct static_int_to_str_constants<char>
{
	static inline char null(void){return '\0';}
	static inline char zero(void){return '0';}
};

template <>
struct static_int_to_str_constants<wchar_t>
{
	static inline wchar_t null(void){return L'\0';}
	static inline wchar_t zero(void){return L'0';}
};

template <typename CharT, int I>
struct static_int_to_str 
: static_int_to_str_constants<CharT>
{
	typedef static_int_to_str_constants<CharT> base_class;
	// the length of the string needed to hold the given integer
	typedef typename mpl::int_<
		static_log10<I>::type::value + 1
	>::type length;
	//
	//
	template <int J>
	static inline CharT get_digit(mpl::int_<J> pos)
	{
		typedef typename static_pow10<
			length::value - mpl::int_<J>::value
		>::type K;
		return base_class::zero() + (I / K::value) % 10;
	}
	//
	static inline void do_copy_to(CharT* _str, mpl::int_<0>){	}
	//
	template <int J>
	static inline void do_copy_to(CharT* _str, mpl::int_<J> pos)
	{
		_str[J-1] = get_digit(pos);
		do_copy_to(_str, mpl::int_<J - 1>());
	}
	//
	static inline CharT* convert(CharT* _str, size_t _max_len)
	{
		assert(_max_len > size_t(length::value));
		do_copy_to(_str, length());
		_str[length::value] = base_class::null();
		return _str;
	}
};

template <typename CharT>
struct static_int_to_str<CharT, 0>
: static_int_to_str_constants<CharT>
{
	typedef static_int_to_str_constants<CharT> base_class;

	typedef mpl::int_<1>::type length;
	static inline CharT* convert(CharT* _str, size_t _max_len)
	{
		assert(_max_len > 1);
		_str[0] = base_class::zero();
		_str[1] = base_class::null();
		return _str;
	}
};

template <class Vector, typename CharT, int I>
struct static_int_to_str_w_prefix
: static_int_to_str_constants<CharT>
{
	typedef static_int_to_str_constants<CharT> base_class;
	//
	typedef Vector prefix;
	typedef mpl::int_<mpl::size<prefix>::value> prefix_length;
	typedef typename static_int_to_str<CharT, I>::length number_length;
	// the length of the string needed to hold the given integer with the prefix
	typedef typename mpl::int_<
		prefix_length::value + 
		number_length::value
	> length;
	//
	static inline void do_apply_prefix_to(CharT* _str, mpl::int_<0>){ }
	//
	template <int J>
	static inline void do_apply_prefix_to(CharT* _str, mpl::int_<J> pos)
	{
		_str[J-1] = mpl::at<
			prefix,
			mpl::int_<J - 1>
		>::type::value;
		do_apply_prefix_to(_str, mpl::int_<J - 1>());
	}
	//
	static CharT* convert(CharT* _str, size_t _max_len)
	{
		// check the length
		assert(_max_len > size_t(length::value));
		// apply prefix
		do_apply_prefix_to(_str, prefix_length());
		// calculate offset
		const int offs = mpl::size<prefix>::type::value;
		// append given int as string
		static_int_to_str<CharT, I>::convert(_str + offs, _max_len - offs);
		// finalize the string
		_str[length::value] = base_class::null();
		return _str;
	}
	//
	struct holder
	{
		static const CharT* get(void)
		{
			static CharT str[length::value+1] = {0};
			if(!str[0]) convert(str, length::value+1);
			return str;
		}
	};

};

} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

