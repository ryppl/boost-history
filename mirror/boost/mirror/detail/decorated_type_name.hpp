/**
 * \file boost/mirror/detail/decorated_type_name.hpp
 *
 * Helpers for composing nontrivial typenames
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_DETAIL_DECORATED_TYPE_NAME_HPP
#define BOOST_MIRROR_META_DETAIL_DECORATED_TYPE_NAME_HPP

#include <boost/mpl/plus.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mirror/detail/static_int_to_str.hpp>

namespace boost {
namespace mirror {
namespace detail {

template <class MetaType, class Decorator>
struct decorated_type_name
{
private:
	template <bool FullName>
	inline static bstring init_name(mpl::bool_<FullName> full_or_base)
	{
		bstring left;
		bstring right;
		bstring temp(build_name(full_or_base, left, right));
		left.append(temp);
		left.append(right);
		return left;
	}


public:
	template <bool FullName>
	inline static bstring build_name(
		mpl::bool_<FullName> full_or_base,
		bstring& left, 
		bstring& right
	)
	{
		Decorator D(left, right);
		return MetaType::build_name(
			full_or_base,
			left,
			right
		);
	}

	template <bool FullName>
	static const bstring& get_name(mpl::bool_<FullName> full_or_base)
	{
		static bstring s_name(init_name(full_or_base));
		return s_name;
	}

	inline static const bstring& base_name(void)
	{
		return get_name(mpl::false_());
	}

	inline static const bstring& full_name(void)
	{
		return get_name(mpl::true_());
	}

};

// no-op decorator
template <typename T>
struct type_name_decorator
{
	inline type_name_decorator(bstring&, bstring&);
};

struct type_name_right_postfix_decorator
{
	inline type_name_right_postfix_decorator(bstring& _r, const bchar* _pfx)
	: right(_r), postfix(_pfx) { }

	inline ~type_name_right_postfix_decorator(void)
	{
		right.append(bstring(postfix));
	}
	bstring& right;
	const bchar* postfix;
};

// pointer decorator
template <typename T>
struct type_name_decorator<T*>
: type_name_right_postfix_decorator
{
	inline type_name_decorator(bstring&, bstring& _right)
	: type_name_right_postfix_decorator(_right, BOOST_STR_LIT(" *"))
	{ }
};


// reference decorator
template <typename T>
struct type_name_decorator<T&>
: type_name_right_postfix_decorator
{
	inline type_name_decorator(bstring&, bstring& _right)
	: type_name_right_postfix_decorator(_right, BOOST_STR_LIT(" &"))
	{ }
};

// const type decorator
template <typename T>
struct type_name_decorator<const T>
: type_name_right_postfix_decorator
{
	inline type_name_decorator(bstring&, bstring& _right)
	: type_name_right_postfix_decorator(_right, BOOST_STR_LIT(" const"))
	{ }
};

// volatile type decorator
template <typename T>
struct type_name_decorator<volatile T>
: type_name_right_postfix_decorator
{
	inline type_name_decorator(bstring&, bstring& _right)
	: type_name_right_postfix_decorator(_right, BOOST_STR_LIT(" volatile"))
	{ }
};

// const volatile type decorator
template <typename T>
struct type_name_decorator<const volatile T>
: type_name_right_postfix_decorator
{
	inline type_name_decorator(bstring&, bstring& _r)
	: type_name_right_postfix_decorator(_r, BOOST_STR_LIT(" const volatile"))
	{ }
};

// array decorator
template <typename T>
struct type_name_decorator< T[] >
{
	inline type_name_decorator(bstring&, bstring& _right)
	{
		_right.append(BOOST_STR_LIT(" []"));
	}
};


// array decorator
template <typename T, size_t Size>
struct type_name_decorator< T[ Size ] >
{
private:
	inline static bstring init_postfix(void)
	{
		typedef typename detail::static_int_to_str<Size>
			size_string;
		// init with '['
		bstring res(BOOST_STR_LIT(" ["));
		// 
		// setup a buffer for the number
		const size_t max_size = size_string::length::value+1;
		bchar buffer[max_size];
		// put it into the buffer
		size_string::convert(buffer, max_size);
		// append the buffer
		res.append(bstring(buffer));
		// append ']'
		res.append(bstring(BOOST_STR_LIT("]")));
		return res;
	}
public:
	inline type_name_decorator(bstring&, bstring& _right)
	{
		static bstring s_postfix(init_postfix());
		_right.append(s_postfix);
	}
};


} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

