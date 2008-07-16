/**
 * \file boost/mirror/detail/template_name.hpp
 * Helpers for composing a template name
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_DETAIL_TEMPLATE_NAME_HPP
#define BOOST_MIRROR_META_DETAIL_TEMPLATE_NAME_HPP

#include <assert.h>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/remove_if.hpp>

#include <boost/mirror/detail/decorated_type_name.hpp>
#include <boost/mirror/detail/argument_type_list.hpp>

namespace boost {
namespace mirror {
namespace detail {

template <class Scope, class BaseMetaObject, class TplArgTypeList>
struct static_template_name_base  : argument_type_list_builder
{
private:
	typedef full_name_builder<
		Scope,
		BaseMetaObject
	> base_meta_template;


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
		typedef type_name_decorator_literals<CharT> lits;
		static ::std::basic_string<CharT> comma(lits::get(lits::comma()));
		static ::std::basic_string<CharT> l_angle(lits::get(lits::langle()));
		static ::std::basic_string<CharT> r_angle(lits::get(lits::rangle()));
		//
		::std::char_traits<CharT> cht;
		//
		// get the template name
		::std::basic_string<CharT> res(base_meta_template::get_name(
			full_or_base,
			cht
		));
		// argument list
		res.append(l_angle);
		append_args(
			((TplArgTypeList*)0), 
			res, 
			full_or_base,
			cht
		);
		res.append(r_angle);
		//
		return res;
	}

};

/** 
 */
template <class Scope, class BaseMetaObject, class TplArgTypeList>
struct static_template_name
: decorated_type_name_finisher<
	static_template_name_base<Scope, BaseMetaObject, TplArgTypeList>
>
{
	typedef Scope scope;
};


} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

