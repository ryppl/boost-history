/**
 * \file boost/mirror/meta_classes/boost_tuple.hpp
 * Meta-class for boost::tuple<T0, T1, ..., Tn>
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_CLASSES_BOOST_TUPLE_HPP
#define BOOST_MIRROR_META_CLASSES_BOOST_TUPLE_HPP

#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/meta_types/boost_tuple.hpp>
#include <boost/mirror/detail/static_int_to_str.hpp>

namespace boost {
namespace mirror {

BOOST_MIRROR_REG_TEMPLATE_ATTRIBS_BEGIN(::boost::tuples::tuple, 10)
	__dummy_list;
	//
	//
	// the full list tuple template arguments
	// which is the list of types of it's member attributes
	typedef typename mpl::vector10< BOOST_PP_ENUM_PARAMS(10, T) > all_template_params;
	// the list of member attributes without the null types
	typedef typename detail::template_with_null_args_type_list<
		all_template_params
	>::type template_params;
	//
	// member attribute base name getter
	template <int I>
	static const bchar* base_name(mpl::int_<I>)
	{
		typedef typename boost::mirror::detail::static_int_to_str_w_prefix<
			mpl::vector_c<bchar, BOOST_STR_LIT('_') >, I
		>::holder name;
		// 
		return name::get();
	}

	template <int I>
	struct att_val_pass
	{
		typedef typename call_traits<
			typename mpl::at<template_params, mpl::int_<I> >::type
		>::param_type type;
	};

	// member attrib value get
	template <int I> 
	static typename att_val_pass<I>::type
	get(const Class& a_tuple, mpl::int_<I>)
	{
		return ::boost::tuples::get<I>(a_tuple);
	}
	
	// member attrib value query
	template <typename dest_type, int I>
	static dest_type& query(const Class& a_tuple, mpl::int_<I>, dest_type& dest)
	{
		dest = dest_type(::boost::tuples::get<I>(a_tuple));
		return dest;
	}
	
	// member attrib value set
	template <int I>
	static void set(Class& a_tuple, mpl::int_<I>, typename att_val_pass<I>::type val)
	{
		::boost::tuples::get<I>(a_tuple) = val;
	}

	// member attrib value set
	template <int I>
	static void set(const Class& a_tuple, mpl::int_<I>, typename att_val_pass<I>::type val)
	{
	}
	//

	// traits getter
	template <int I>
	static meta_class_attribute_traits<false> 
	get_traits(mpl::int_<I>);

	//
	typedef template_params
BOOST_MIRROR_REG_TEMPLATE_ATTRIBS_END


} // namespace mirror
} // namespace boost

#endif //include guard

