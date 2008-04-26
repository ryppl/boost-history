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
#include <set> 

namespace boost {
namespace mirror {

namespace detail {

template <typename K, class Comp, class Allocator> 
struct meta_type_std_set
{

	static const bchar* base_name(void){return BOOST_STR_LIT("set");}
	BOOST_MIRROR_CONST_MEMBER_ATTRIB(
		size_t,
		base_name_length,
		3
	)

	static const bchar* full_name(void){return BOOST_STR_LIT("::std::set");}
	BOOST_MIRROR_CONST_MEMBER_ATTRIB(
		size_t,
		full_name_length,
		10
	)
};

} // namespace detail


template <typename K, class Comp, class Allocator> 
struct meta_type< ::std::set<K, Comp, Allocator> > 
: detail::static_template_name<
	detail::meta_type_std_set<K, Comp, Allocator> , 
	mpl::vector1<K> // TODO add Comp too
>
{
	typedef BOOST_MIRROR_REFLECT_NAMESPACE(_std) scope;
	typedef ::std::set<K, Comp, Allocator> base_type;
};


} // namespace mirror
} // namespace boost

#endif //include guard

