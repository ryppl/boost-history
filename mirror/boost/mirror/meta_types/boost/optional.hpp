/**
 * \file boost/mirror/meta_types/boost/optional.hpp
 *
 * Meta-type for boost::optional<T>
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_TYPES_BOOST_OPTIONAL_HPP
#define BOOST_MIRROR_META_TYPES_BOOST_OPTIONAL_HPP

#include <boost/mirror/meta_type.hpp>
#include <boost/mirror/detail/template_name.hpp>
#include <boost/optional/optional.hpp> 

namespace boost {
namespace mirror {

namespace detail {

struct meta_type_boost_optional
{
	BOOST_MIRROR_REG_TYPE_DECLARE_BASE_NAME("optional")
};

} // namespace detail


template <typename T> 
struct meta_type< ::boost::optional<T> > 
: detail::static_template_name<
	BOOST_MIRRORED_NAMESPACE(::boost),
	detail::meta_type_boost_optional , 
	mpl::vector1<T>
>
{
	typedef ::boost::optional<T> reflected_type;
};


} // namespace mirror
} // namespace boost

#endif //include guard

