/**
 * \file boost/mirror/meta_types/std_vector.hpp
 * Meta-type for std::vector<T, ...>
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_TYPES_STD_VECTOR_HPP
#define BOOST_MIRROR_META_TYPES_STD_VECTOR_HPP

#include <boost/mirror/meta_type.hpp>
#include <boost/mirror/detail/template_name.hpp>
#include <vector> 

namespace boost {
namespace mirror {

namespace detail {

template <typename T, class Allocator> 
struct meta_type_std_vector
{

	static const bchar* base_name(void){return BOOST_STR_LIT("vector");}
	BOOST_STATIC_CONSTANT(int, base_name_length = 6);

	static const bchar* full_name(void){return BOOST_STR_LIT("::std::vector");}
	BOOST_STATIC_CONSTANT(int, full_name_length = 13);
};

} // namespace detail


template <typename T, class Allocator> 
struct meta_type< ::std::vector<T, Allocator> > 
: detail::static_template_name<
	detail::meta_type_std_vector<T, Allocator> , 
	mpl::vector1<T>
>
{
	typedef BOOST_MIRROR_REFLECT_NAMESPACE(_std) scope;                        
	typedef ::std::vector<T, Allocator> reflected_type;
};


} // namespace mirror
} // namespace boost

#endif //include guard

