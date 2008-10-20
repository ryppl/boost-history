/**
 * \file boost/mirror/meta_types/boost/tuples/tuple.hpp
 * Meta-type for boost::tuple<>
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_TYPES_BOOST_TUPLES_TUPLE_HPP
#define BOOST_MIRROR_META_TYPES_BOOST_TUPLES_TUPLE_HPP

#include <boost/mirror/meta_type.hpp>
#include <boost/mirror/detail/template_name.hpp>
#include <boost/mirror/meta_namespaces/boost/tuples.hpp>
//
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
	BOOST_MIRROR_REG_TYPE_DECLARE_BASE_NAME("tuple")
};


// designate tuples::null_type as typelist null type
template <>
struct is_typelist_null_type< ::boost::tuples::null_type>
	: ::boost::true_type { };


} // namespace detail


BOOST_MIRROR_TMP_BOOST_TUPLE_TEMPL_DECL()
struct meta_type< 
	::boost::tuples::tuple< BOOST_MIRROR_TMP_BOOST_TUPLE_TEMPL_ARG_NAMES() > 
> 
: detail::static_template_name<
	BOOST_MIRRORED_NAMESPACE(::boost::tuples),
	detail::meta_type_boost_tuple<
		BOOST_MIRROR_TMP_BOOST_TUPLE_TEMPL_ARG_NAMES()
	>, 
	mpl::vector<
		BOOST_MIRROR_TMP_BOOST_TUPLE_TEMPL_ARG_NAMES()
	>
>
{
	typedef ::boost::tuples::tuple< 
		BOOST_MIRROR_TMP_BOOST_TUPLE_TEMPL_ARG_NAMES() 
	> reflected_type;
};

// undefine the helper macros
#undef BOOST_MIRROR_TMP_BOOST_TUPLE_TEMPL_DECL
#undef BOOST_MIRROR_TMP_BOOST_TUPLE_TEMPL_ARG_NAMES


} // namespace mirror
} // namespace boost

#endif //include guard

