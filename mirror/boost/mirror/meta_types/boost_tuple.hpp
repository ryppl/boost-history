/**
 * \file boost/mirror/meta_types/boost_tuple.hpp
 * Meta-type for boost::tuple<>
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_TYPES_BOOST_TUPLE_HPP
#define BOOST_MIRROR_META_TYPES_BOOST_TUPLE_HPP

#include <boost/mirror/meta_type.hpp>
#include <boost/mirror/detail/template_name.hpp>
#include <boost/mirror/meta_namespaces/boost_tuples.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

namespace boost {
namespace mirror {

#define BOOST_MIRROR_TMP_BOOST_TUPLE_TEMPL_DECL() \
template < \
	BOOST_PP_ENUM_PARAMS(10, class T) \
>

#define BOOST_MIRROR_TMP_BOOST_TUPLE_TEMPL_ARG_NAMES() \
	BOOST_PP_ENUM_PARAMS(10, T)


namespace detail {

BOOST_MIRROR_TMP_BOOST_TUPLE_TEMPL_DECL()
struct meta_type_boost_tuple 
{

	static const bchar* base_name(void){return BOOST_STR_LIT("tuple");}
	BOOST_MIRROR_CONST_MEMBER_ATTRIB(
		size_t,
		base_name_length,
		5
	)

	static const bchar* full_name(void){return BOOST_STR_LIT("::boost::tuples::tuple");}
	BOOST_MIRROR_CONST_MEMBER_ATTRIB(
		size_t,
		full_name_length,
		22
	)
};


// designate tuples::null_type as typelist null type
template <>
struct is_typelist_null_type<::boost::tuples::null_type> : ::boost::true_type { };


} // namespace detail


BOOST_MIRROR_TMP_BOOST_TUPLE_TEMPL_DECL()
struct meta_type< 
	::boost::tuples::tuple< BOOST_MIRROR_TMP_BOOST_TUPLE_TEMPL_ARG_NAMES() > 
> 
: detail::static_template_name<
	detail::meta_type_boost_tuple<
		BOOST_MIRROR_TMP_BOOST_TUPLE_TEMPL_ARG_NAMES()
	>, 
	mpl::vector<
		BOOST_MIRROR_TMP_BOOST_TUPLE_TEMPL_ARG_NAMES()
	>
>
{
	typedef BOOST_MIRROR_REFLECT_NAMESPACE(_boost_tuples) scope;                        
	typedef ::boost::tuples::tuple< 
		BOOST_MIRROR_TMP_BOOST_TUPLE_TEMPL_ARG_NAMES() 
	> base_type;
};

// undefine the helper macros
#undef BOOST_MIRROR_TMP_BOOST_TUPLE_TEMPL_DECL
#undef BOOST_MIRROR_TMP_BOOST_TUPLE_TEMPL_ARG_NAMES


} // namespace mirror
} // namespace boost

#endif //include guard

