/**
 * \file boost/mirror/meta_classes/boost_fusion_vector.hpp
 * Meta-class for boost::fusion::vector<T0, T1, ..., Tn>
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_CLASSES_BOOST_FUSION_VECTOR_HPP
#define BOOST_MIRROR_META_CLASSES_BOOST_FUSION_VECTOR_HPP

#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/meta_types/boost_fusion_vector.hpp>
#include <boost/mirror/detail/static_int_to_str.hpp>
#include <boost/mirror/detail/unnamed_attribs.hpp>
#include <boost/fusion/sequence/intrinsic/at.hpp>


namespace boost {
namespace mirror {


BOOST_MIRROR_REG_TEMPLATE_ATTRIBS_BEGIN(
	::boost::fusion::vector, 
	FUSION_MAX_VECTOR_SIZE
) 
	__dummy_list;
	//
	//
	// the full list vectors template arguments
	typedef typename mpl::vector< 
		BOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, T) 
	> all_template_params;
	//
	// the list of member attributes without the null types
	typedef typename detail::template_args_type_list_wo_nulls<
		all_template_params
	>::type template_params;
	//
	template <int I>
	struct att_val_pass
	{
		typedef typename call_traits<
			typename mpl::at<template_params, mpl::int_<I> >::type
		>::param_type type;
	};
	//
	// member attribute base name getter
	template <int I, typename CharT>
	inline static const ::std::basic_string<CharT>& get_name(
		mpl::int_<I> pos, 
		mpl::false_ full_name,
		::std::char_traits<CharT> cht
	)
	{
		// get the prefix 'static string'
		typedef typename detail::meta_class_tuple_attrib_name_prefix<
			CharT
		>::type prefix;
		// get a prefixed name builder
		typedef typename boost::mirror::detail::static_int_to_str_w_prefix<
			prefix,
			CharT,
			I
		>::holder name_builder;
		// build the name and assing it to the static name
		static ::std::basic_string<CharT> name(name_builder::get());
		// return the built name
		return name;
	}

	// traits getter
	template <int I>
	static meta_class_attribute_traits<
		::boost::mirror::attrib_storage_specifiers::__,
		typename att_val_pass<I>::type
	> get_traits(mpl::int_<I>);

	
	// member attrib value get
	template <int I> 
	static typename att_val_pass<I>::type
	get(const Class& an_inst, mpl::int_<I>)
	{
		return ::boost::fusion::at_c<I>(an_inst);
	}
	
	// member attrib value query
	template <typename dest_type, int I>
	static dest_type& query(const Class& an_inst, mpl::int_<I>, dest_type& dest)
	{
		dest = dest_type(::boost::fusion::at_c<I>(an_inst));
		return dest;
	}
	
	// member attrib value set
	template <int I>
	static void set(Class& an_inst, mpl::int_<I>, typename att_val_pass<I>::type val)
	{
		::boost::fusion::at_c<I>(an_inst) = val;
	}

	// const member attrib value set
	template <int I>
	static void set(const Class& an_inst, mpl::int_<I>, typename att_val_pass<I>::type val)
	{
	}

	//
	typedef template_params
BOOST_MIRROR_REG_TEMPLATE_ATTRIBS_END


} // namespace mirror
} // namespace boost

#endif //include guard

