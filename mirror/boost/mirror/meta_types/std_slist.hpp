/**
 * \file boost/mirror/meta_types/std_slist.hpp
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

	static const bchar* base_name(void){return BOOST_STR_LIT("slist");}
	BOOST_STATIC_CONSTANT(int, base_name_length = 5);

	static const bchar* full_name(void){return BOOST_STR_LIT("::std::slist");}
	BOOST_STATIC_CONSTANT(int, full_name_length = 12);
};

} // namespace detail


template <typename T, class Allocator> 
struct meta_type< ::std::slist<T, Allocator> > 
: detail::static_template_name<
	detail::meta_type_std_slist<T, Allocator> , 
	mpl::vector1<T>
>
{
	typedef BOOST_MIRROR_REFLECT_NAMESPACE(_std) scope;                        
	typedef ::std::slist<T, Allocator> base_type;
};


} // namespace mirror
} // namespace boost

#endif //include guard

