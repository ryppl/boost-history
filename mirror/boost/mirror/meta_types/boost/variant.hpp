/**
 * \file boost/mirror/meta_types/boost/variant.hpp
 * Meta-type for boost::variant<>
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_TYPES_BOOST_VARIANT_HPP
#define BOOST_MIRROR_META_TYPES_BOOST_VARIANT_HPP

#include <boost/mirror/meta_type.hpp>
#include <boost/mirror/detail/template_name.hpp>
//
#include <boost/variant.hpp>

namespace boost {
namespace mirror {

#define BOOST_MIRROR_TMP_BOOST_VARIANT_TEMPL_DECL() \
template < \
	BOOST_VARIANT_ENUM_PARAMS(typename T)\
>

#define BOOST_MIRROR_TMP_BOOST_VARIANT_TEMPL_ARG_NAMES() \
	BOOST_VARIANT_ENUM_PARAMS(T)


namespace detail {

struct meta_type_boost_variant 
{
	BOOST_MIRROR_REG_TYPE_DECLARE_BASE_NAME("variant")
};


// designate detail::variant::void_ as typelist null type
template <>
struct is_typelist_null_type< ::boost::detail::variant::void_ >
	: ::boost::true_type { };


} // namespace detail


BOOST_MIRROR_TMP_BOOST_VARIANT_TEMPL_DECL()
struct meta_type< 
	::boost::variant< BOOST_MIRROR_TMP_BOOST_VARIANT_TEMPL_ARG_NAMES() > 
> 
: detail::static_template_name<
	BOOST_MIRRORED_NAMESPACE(::boost),
	detail::meta_type_boost_variant, 
	mpl::vector<
		BOOST_MIRROR_TMP_BOOST_VARIANT_TEMPL_ARG_NAMES()
	>
>
{
	typedef ::boost::variant< 
		BOOST_MIRROR_TMP_BOOST_VARIANT_TEMPL_ARG_NAMES() 
	> reflected_type;
};

// undefine the helper macros
#undef BOOST_MIRROR_TMP_BOOST_VARIANT_TEMPL_DECL
#undef BOOST_MIRROR_TMP_BOOST_VARIANT_TEMPL_ARG_NAMES


} // namespace mirror
} // namespace boost

#endif //include guard

