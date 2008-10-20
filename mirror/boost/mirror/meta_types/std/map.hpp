/**
 * \file boost/mirror/meta_types/std/map.hpp
 * Meta-type for std::map<K, T, ...>
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_TYPES_STD_MAP_HPP
#define BOOST_MIRROR_META_TYPES_STD_MAP_HPP

#include <boost/mirror/meta_type.hpp>
#include <boost/mirror/detail/template_name.hpp>
#include <boost/mirror/meta_types/std/less.hpp>
#include <map> 

namespace boost {
namespace mirror {

namespace detail {

template <typename K, typename T, class Comp, class Allocator> 
struct meta_type_std_map
{
	BOOST_MIRROR_REG_TYPE_DECLARE_BASE_NAME("map")
};

} // namespace detail


template <typename K, typename T, class Comp, class Allocator> 
struct meta_type< ::std::map<K, T, Comp, Allocator> > 
: detail::static_template_name<
	BOOST_MIRRORED_NAMESPACE(::std),
	detail::meta_type_std_map<K, T, Comp, Allocator> , 
	mpl::vector3<K, T, Comp> 
>
{
	typedef ::std::map<K, T, Comp, Allocator> reflected_type;
};


} // namespace mirror
} // namespace boost

#endif //include guard

