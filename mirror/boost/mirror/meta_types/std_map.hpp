/**
 * \file boost/mirror/meta_types/std_map.hpp
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
#include <map> 

namespace boost {
namespace mirror {

namespace detail {

template <typename K, typename T, class Comp, class Allocator> 
struct meta_type_std_map
{

	static const bchar* base_name(void){return BOOST_STR_LIT("map");}
	BOOST_STATIC_CONSTANT(int, base_name_length = 3);

	static const bchar* full_name(void){return BOOST_STR_LIT("::std::map");}
	BOOST_STATIC_CONSTANT(int, full_name_length = 10);
};

} // namespace detail


template <typename K, typename T, class Comp, class Allocator> 
struct meta_type< ::std::map<K, T, Comp, Allocator> > 
: detail::static_template_name<
	detail::meta_type_std_map<K, T, Comp, Allocator> , 
	mpl::vector2<K, T> // TODO add Comp too
>
{
	typedef BOOST_MIRROR_REFLECT_NAMESPACE(_std) scope;
	typedef ::std::map<K, T, Comp, Allocator> reflected_type;
};


} // namespace mirror
} // namespace boost

#endif //include guard

