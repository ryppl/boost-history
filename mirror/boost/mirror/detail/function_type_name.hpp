/**
 * \file boost/mirror/detail/function_type_name.hpp
 *
 * Helpers for composing function typenames
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_DETAIL_FUNCTION_TYPE_NAME_HPP
#define BOOST_MIRROR_META_DETAIL_FUNCTION_TYPE_NAME_HPP

#include <boost/mpl/for_each.hpp>
#include <boost/mpl/pop_back.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/identity.hpp>
//
#include <boost/mirror/meta_data_fwd.hpp>
#include <boost/mirror/detail/decorated_type_name.hpp>
#include <boost/mirror/detail/argument_type_list.hpp>

namespace boost {
namespace mirror {
namespace detail {

/** Function type name builder 
 */
template <
	typename RetValType, 
	typename CallingConvention, 
	typename ArgTypeList
>
struct function_type_name_base : argument_type_list_builder
{
public:
	template <bool FullName, typename CharT>
	static ::std::basic_string<CharT> build_name(
		mpl::bool_<FullName> full_or_base,
		::std::basic_string<CharT>& left, 
		::std::basic_string<CharT>& right,
		::std::basic_string<CharT>& ex,
		::std::basic_string<CharT>& arg
	)
	{
		typedef type_name_decorator_literals<CharT> lits;
		static ::std::basic_string<CharT> space(lits::get(lits::space()));
		static ::std::basic_string<CharT> l_par(lits::get(lits::lpar()));
		static ::std::basic_string<CharT> r_par(lits::get(lits::rpar()));
	//
		// the return value type
		typedef BOOST_MIRRORED_TYPE(RetValType) meta_RV;
		//
		::std::basic_string<CharT> rv_left;
		::std::basic_string<CharT> rv_right;
		::std::basic_string<CharT> rv_ex;
		::std::basic_string<CharT> rv_arg;
		::std::basic_string<CharT> rv_t(meta_RV::build_name(
			full_or_base,
			rv_left,
			rv_right,
			rv_ex,
			rv_arg
		));
		::std::char_traits<CharT> cht;
		//
		// return value 
		left.append(rv_left);
		left.append(rv_t);
		left.append(rv_right);
		left.append(rv_ex);
		// open pars.
		left.append(space);
		left.append(l_par);
		// the calling convention
		left.append(CallingConvention::name(cht));
		arg.append(r_par);
		//
		// argument list
		arg.append(l_par);
		append_args(
			((ArgTypeList*)0), 
			arg, 
			full_or_base, 
			cht
		);
		arg.append(r_par);
		// the rest of the ret val type
		arg.append(rv_arg);
		// return an empty string
		return ::std::basic_string<CharT>();
	}
};

template <
	typename RetValType, 
	typename CallingConvention, 
	typename ArgTypeList
>
struct function_type_name
: decorated_type_name_finisher<
	function_type_name_base<RetValType, CallingConvention, ArgTypeList>
> { };



} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

