/**
 * \file boost/mirror/meta_types/std_less.hpp
 *
 * Meta-type for std::less<T>
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_TYPES_STD_LESS_HPP
#define BOOST_MIRROR_META_TYPES_STD_LESS_HPP

#include <boost/mirror/meta_type.hpp>
#include <boost/mirror/detail/template_name.hpp>
#include <functional> 

namespace boost {
namespace mirror {

namespace detail {

template <typename > 
struct meta_type_std_less
{
	BOOST_MIRROR_REG_TYPE_DECLARE_BASE_NAME("less")
};

} // namespace detail


template <typename T> 
struct meta_type< ::std::less<T> > 
: detail::static_template_name<
	BOOST_MIRRORED_NAMESPACE(::std),
	detail::meta_type_std_less<T> , 
	mpl::vector1<T>
>
{
	typedef ::std::less<T> reflected_type;
};


} // namespace mirror
} // namespace boost

#endif //include guard

