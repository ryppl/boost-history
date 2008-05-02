/**
 * \file boost/mirror/meta_types/std_pair.hpp
 * Meta-type for std::pair<T1, T2>
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_TYPES_STD_PAIR_HPP
#define BOOST_MIRROR_META_TYPES_STD_PAIR_HPP

#include <boost/mirror/meta_type.hpp>
#include <boost/mirror/detail/template_name.hpp>
//std::pair
#include <utility> 

namespace boost {
namespace mirror {

namespace detail {

template <typename FirstType, typename SecondType> 
struct meta_type_std_pair 
{

	static const bchar* base_name(void){return BOOST_STR_LIT("pair");}
	BOOST_STATIC_CONSTANT(int, base_name_length = 4);

	static const bchar* full_name(void){return BOOST_STR_LIT("::std::pair");}
	BOOST_STATIC_CONSTANT(int, full_name_length = 11);
};

} // namespace detail


template <typename FirstType, typename SecondType> 
struct meta_type< ::std::pair<FirstType, SecondType> > 
: detail::static_template_name<
	detail::meta_type_std_pair<FirstType, SecondType> , 
	mpl::vector2<FirstType, SecondType>
>
{
	typedef BOOST_MIRROR_REFLECT_NAMESPACE(_std) scope;                        
	typedef ::std::pair<FirstType, SecondType> reflected_type;
};


} // namespace mirror
} // namespace boost

#endif //include guard

