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

template <typename first_type, typename second_type> 
struct meta_type_std_pair 
{

	static const bchar* base_name(void){return BOOST_STR_LIT("pair");}
	BOOST_MIRROR_CONST_MEMBER_ATTRIB(
		size_t,
		base_name_length,
		4
	)

	static const bchar* full_name(void){return BOOST_STR_LIT("::std::pair");}
	BOOST_MIRROR_CONST_MEMBER_ATTRIB(
		size_t,
		full_name_length,
		11
	)
};

} // namespace detail


template <typename first_type, typename second_type> 
struct meta_type< ::std::pair<first_type, second_type> > 
: detail::static_template_name<
	detail::meta_type_std_pair<first_type, second_type> , 
	mpl::vector2<first_type, second_type>
>
{
	typedef BOOST_MIRROR_REFLECT_NAMESPACE(_std) scope;                        
	typedef ::std::pair<first_type, second_type> base_type;
};


} // namespace mirror
} // namespace boost

#endif //include guard

