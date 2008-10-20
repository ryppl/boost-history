/**
 * \file boost/mirror/meta_types/std_set.hpp
 * Meta-type for std::set<K,, ...>
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_TYPES_STD_SET_HPP
#define BOOST_MIRROR_META_TYPES_STD_SET_HPP

#include <boost/mirror/meta_type.hpp>
#include <boost/mirror/detail/template_name.hpp>
#include <boost/mirror/meta_types/std_less.hpp>
#include <set> 

namespace boost {
namespace mirror {

namespace detail {

template <typename K, class Comp, class Allocator> 
struct meta_type_std_set
{
	BOOST_MIRROR_REG_TYPE_DECLARE_BASE_NAME("set")
};

} // namespace detail


template <typename K, class Comp, class Allocator> 
struct meta_type< ::std::set<K, Comp, Allocator> > 
: detail::static_template_name<
	BOOST_MIRRORED_NAMESPACE(::std),
	detail::meta_type_std_set<K, Comp, Allocator> , 
	mpl::vector2<K, Comp>
>
{
	typedef ::std::set<K, Comp, Allocator> reflected_type;
};


} // namespace mirror
} // namespace boost

#endif //include guard

