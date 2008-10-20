/**
 * \file boost/mirror/meta_types/boost/fusion/vector.hpp
 * Meta-type for boost::fusion::vector<>
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_TYPES_BOOST_FUSION_VECTOR_HPP
#define BOOST_MIRROR_META_TYPES_BOOST_FUSION_VECTOR_HPP

#include <boost/mirror/meta_type.hpp>
#include <boost/mirror/detail/template_name.hpp>
#include <boost/mirror/meta_namespaces/boost/fusion.hpp>
//
#include <boost/fusion/container/vector.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

namespace boost {
namespace mirror {

#define BOOST_MIRROR_TMP_BOOST_FUSION_VECTOR_TEMPL_DECL() \
template < \
	BOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, class T) \
>

#define BOOST_MIRROR_TMP_BOOST_FUSION_VECTOR_TEMPL_ARG_NAMES() \
	BOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, T)

namespace detail {

BOOST_MIRROR_TMP_BOOST_FUSION_VECTOR_TEMPL_DECL()
struct meta_type_boost_fusion_vector 
{
	BOOST_MIRROR_REG_TYPE_DECLARE_BASE_NAME("vector")
};


// designate fusion::void_ type as a typelist null type
template <>
struct is_typelist_null_type< ::boost::fusion::void_ >
	: ::boost::true_type { };


} // namespace detail

BOOST_MIRROR_TMP_BOOST_FUSION_VECTOR_TEMPL_DECL()
struct meta_type< 
	::boost::fusion::vector< BOOST_MIRROR_TMP_BOOST_FUSION_VECTOR_TEMPL_ARG_NAMES() > 
> 
: detail::static_template_name<
	BOOST_MIRRORED_NAMESPACE(::boost::fusion),
	detail::meta_type_boost_fusion_vector<
		BOOST_MIRROR_TMP_BOOST_FUSION_VECTOR_TEMPL_ARG_NAMES()
	>, 
	mpl::vector<
		BOOST_MIRROR_TMP_BOOST_FUSION_VECTOR_TEMPL_ARG_NAMES()
	>
>
{
	typedef ::boost::fusion::vector< 
		BOOST_MIRROR_TMP_BOOST_FUSION_VECTOR_TEMPL_ARG_NAMES() 
	> reflected_type;
};

// undefine the helper macros
#undef BOOST_MIRROR_TMP_BOOST_FUSION_VECTOR_TEMPL_DECL
#undef BOOST_MIRROR_TMP_BOOST_FUSION_VECTOR_TEMPL_ARG_NAMES


} // namespace mirror
} // namespace boost

#endif //include guard

