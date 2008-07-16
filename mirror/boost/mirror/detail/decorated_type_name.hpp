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
struct decorated_type_name_base
{
public:
	template <bool FullName, typename CharT>
	inline static ::std::basic_string<CharT> build_name(
		mpl::bool_<FullName> full_or_base,
		::std::basic_string<CharT>& left, 
		::std::basic_string<CharT>& right,
		::std::basic_string<CharT>& ex,
		::std::basic_string<CharT>& arg
	)
	{
		typename Decorator::template get<CharT>::type D(
			left, 
			right, 
			ex, 
			arg
		);
		return MetaType::build_name(
			full_or_base,
			left,
			right,
			ex,
			arg
		);
	}


};

template <class Base>
struct decorated_type_name_finisher : public Base
{
protected:
	template <bool FullName, typename CharT>
	inline static ::std::basic_string<CharT> init_name(
		mpl::bool_<FullName> full_or_base,
		::std::char_traits<CharT> _cht
	)
	{
		::std::basic_string<CharT> left;
		::std::basic_string<CharT> right;
		::std::basic_string<CharT> ex;
		::std::basic_string<CharT> arg;
		::std::basic_string<CharT> temp(
			build_name(full_or_base, left, right, ex, arg)
		);
		left.append(temp);
		left.append(right);
		left.append(ex);
		left.append(arg);
		return left;
	}
public:
	template <bool FullName, typename CharT>
	static const ::std::basic_string<CharT>& get_name(
		mpl::bool_<FullName> full_or_base,
		::std::char_traits<CharT> _cht
	)
	{
		static ::std::basic_string<CharT> s_name(
			init_name(full_or_base, _cht)
		);
		return s_name;
	}

	inline static const cts::bstring& base_name(void)
	{
		return get_name(
			mpl::false_(),
			cts::bchar_traits()
		);
	}

	inline static const cts::bstring& full_name(void)
	{
		return get_name(
			mpl::true_(),
			cts::bchar_traits()
		);
	}
};

/** This template should be used to decorate typenames
 *  using custom decorators.
 */
template <class MetaType, class Decorator>
struct decorated_type_name
: decorated_type_name_finisher<
	decorated_type_name_base<MetaType, Decorator>
> { };

struct type_name_decorator_literal_selectors
{
	struct space { };
	struct asterisk_post { };
	struct ampersand_post { };
	struct const_post { };
	struct volatile_post { };
	struct cv_post { };
	struct lbracket { };
	struct rbracket { };
	struct brackets { };
	struct lpar { };
	struct rpar { };
	struct comma { };
	struct langle { };
	struct rangle { };
};

/** Declaration of a helper template that 
 *  has functions returning the common decorating
 *  literals.
 */
template <typename CharT>
struct type_name_decorator_literals;

// specialization for chars
template <>
struct type_name_decorator_literals<char>
: type_name_decorator_literal_selectors
{
	typedef type_name_decorator_literal_selectors bc;
	inline static const char* get(bc::space){return " ";}
	inline static const char* get(bc::asterisk_post){return " *";}
	inline static const char* get(bc::ampersand_post){return " &";}
	inline static const char* get(bc::const_post){return " const";}
	inline static const char* get(bc::volatile_post){return " volatile";}
	inline static const char* get(bc::cv_post){return " const volatile";}
	inline static const char* get(bc::lbracket){return "[";}
	inline static const char* get(bc::rbracket){return "]";}
	inline static const char* get(bc::brackets){return "[]";}
	inline static const char* get(bc::lpar){return "(";}
	inline static const char* get(bc::rpar){return ")";}
	inline static const char* get(bc::comma){return ", ";}
	inline static const char* get(bc::langle){return "< ";}
	inline static const char* get(bc::rangle){return " >";}
};

// specialization for chars
template <>
struct type_name_decorator_literals<wchar_t>
: type_name_decorator_literal_selectors
{
	typedef type_name_decorator_literal_selectors bc;
	inline static const wchar_t* get(bc::space){return L" ";}
	inline static const wchar_t* get(bc::asterisk_post){return L" *";}
	inline static const wchar_t* get(bc::ampersand_post){return L" &";}
	inline static const wchar_t* get(bc::const_post){return L" const";}
	inline static const wchar_t* get(bc::volatile_post){return L" volatile";}
	inline static const wchar_t* get(bc::cv_post){return L" const volatile";}
	inline static const wchar_t* get(bc::lbracket){return L"[";}
	inline static const wchar_t* get(bc::rbracket){return L"]";}
	inline static const wchar_t* get(bc::brackets){return L"[]";}
	inline static const wchar_t* get(bc::lpar){return L"(";}
	inline static const wchar_t* get(bc::rpar){return L")";}
	inline static const wchar_t* get(bc::comma){return L", ";}
	inline static const wchar_t* get(bc::langle){return L"< ";}
	inline static const wchar_t* get(bc::rangle){return L" >";}
};

// no-op decorator
template <typename T>
struct type_name_decorator;

/** Base class for decorators that append something to 'right'
 *  part of the type name
 */
template <typename CharT, class Selector>
struct type_name_right_postfix_decorator_impl
	: type_name_decorator_literals<CharT>
{
	inline type_name_right_postfix_decorator_impl(
		::std::basic_string<CharT>&, 
		::std::basic_string<CharT>& _right, 
		::std::basic_string<CharT>&, 
		::std::basic_string<CharT>&
	) : right(_right) { }

	inline ~type_name_right_postfix_decorator_impl(void)
	{
		right.append(
			::std::basic_string<CharT>(
				type_name_decorator_literals<CharT>::get(Selector())
			)
		);
	}
	::std::basic_string<CharT>& right;
};

template <class Selector>
struct type_name_right_postfix_decorator
{
	template <typename CharT>
	struct get
	{
		typedef type_name_right_postfix_decorator_impl<
			CharT,
			Selector
		> type;
	};
};


// pointer decorator
template <typename T>
struct type_name_decorator<T*>
: type_name_right_postfix_decorator<
	type_name_decorator_literal_selectors::asterisk_post
> { };

// reference decorator
template <typename T>
struct type_name_decorator<T&>
: type_name_right_postfix_decorator<
	type_name_decorator_literal_selectors::ampersand_post
> { };

// const type decorator
template <typename T>
struct type_name_decorator<const T>
: type_name_right_postfix_decorator<
	type_name_decorator_literal_selectors::const_post
> { };

// volatile type decorator
template <typename T>
struct type_name_decorator<volatile T>
: type_name_right_postfix_decorator<
	type_name_decorator_literal_selectors::volatile_post
> { };

// const volatile type decorator
template <typename T>
struct type_name_decorator<const volatile T>
: type_name_right_postfix_decorator<
	type_name_decorator_literal_selectors::cv_post
> { };

// array decorator
template <typename T>
struct type_name_decorator< T[] >
{
	template <typename CharT>
	struct get : type_name_decorator_literals<CharT>
	{
		typedef type_name_decorator_literals<CharT> bc;
		inline get(
			::std::basic_string<CharT>&, 
			::std::basic_string<CharT>&, 
			::std::basic_string<CharT>& _ex, 
			::std::basic_string<CharT>&
		)
		{
			if(_ex.empty()) _ex.append(bc::get(bc::space()));
			_ex.append(bc::get(bc::brackets()));
		}

		typedef get<CharT> type;
	};
};


// array decorator
template <typename T, size_t Size>
struct type_name_decorator< T[ Size ] >
{
	template <typename CharT>
	struct get : type_name_decorator_literals<CharT>
	{
	private:
		typedef type_name_decorator_literals<CharT> bc;

		inline static ::std::basic_string<CharT> init_postfix(void)
		{
			typedef typename detail::static_int_to_str<CharT, Size>
				size_string;
			// init with '['
			::std::basic_string<CharT> res(bc::get(bc::lbracket()));
			// 
			// setup a buffer for the number
			const size_t max_size = size_string::length::value+1;
			CharT buffer[max_size];
			// put it into the buffer
			size_string::convert(buffer, max_size);
			// append the buffer
			res.append(::std::basic_string<CharT>(buffer));
			// append ']'
			res.append(::std::basic_string<CharT>(bc::get(bc::rbracket())));
			return res;
		}
	public:
		inline get(
			::std::basic_string<CharT>&, 
			::std::basic_string<CharT>&, 
			::std::basic_string<CharT>& _ex, 
			::std::basic_string<CharT>&
		)
		{
			static ::std::basic_string<CharT> s_postfix(init_postfix());
			if(_ex.empty()) 
				_ex.append(::std::basic_string<CharT>(bc::get(bc::space())));
			_ex.append(s_postfix);
		}

		typedef get<CharT> type;
	};
};


} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

