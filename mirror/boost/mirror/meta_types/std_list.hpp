/**
 * \file boost/mirror/meta_types/std_list.hpp
 * Meta-type for std::list<T, ...>
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_TYPES_STD_LIST_HPP
#define BOOST_MIRROR_META_TYPES_STD_LIST_HPP

#include <boost/mirror/meta_type.hpp>
#include <boost/mirror/detail/template_name.hpp>
#include <list> 

namespace boost {
namespace mirror {

namespace detail {

template <typename T, class Allocator> 
struct meta_type_std_list
{

	static const bchar* base_name(void){return BOOST_STR_LIT("list");}
	BOOST_STATIC_CONSTANT(int, base_name_length = 4);

	static const bchar* full_name(void){return BOOST_STR_LIT("::std::list");}
	BOOST_STATIC_CONSTANT(int, full_name_length = 11);
};

} // namespace detail


template <typename T, class Allocator> 
struct meta_type< ::std::list<T, Allocator> > 
: detail::static_template_name<
	detail::meta_type_std_list<T, Allocator> , 
	mpl::vector1<T>
>
{
	typedef BOOST_MIRROR_REFLECT_NAMESPACE(_std) scope;                        
	typedef ::std::list<T, Allocator> base_type;
};


} // namespace mirror
} // namespace boost

#endif //include guard

