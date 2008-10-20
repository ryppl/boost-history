/**
 * \file boost/mirror/meta_types/std/slist.hpp
 * Meta-type for std::list<T, ...>
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_TYPES_STD_SLIST_HPP
#define BOOST_MIRROR_META_TYPES_STD_SLIST_HPP

#include <boost/mirror/meta_type.hpp>
#include <boost/mirror/detail/template_name.hpp>
#include <slist> 

namespace boost {
namespace mirror {

namespace detail {

template <typename T, class Allocator> 
struct meta_type_std_slist
{
	BOOST_MIRROR_REG_TYPE_DECLARE_BASE_NAME("slist")
};

} // namespace detail


template <typename T, class Allocator> 
struct meta_type< ::std::slist<T, Allocator> > 
: detail::static_template_name<
	BOOST_MIRRORED_NAMESPACE(::std),
	detail::meta_type_std_slist<T, Allocator> , 
	mpl::vector1<T>
>
{
	typedef ::std::slist<T, Allocator> reflected_type;
};

} // namespace mirror
} // namespace boost

#endif //include guard

