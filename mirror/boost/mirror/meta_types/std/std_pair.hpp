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
	BOOST_MIRROR_REG_TYPE_DECLARE_BASE_NAME("pair")
};

} // namespace detail


template <typename FirstType, typename SecondType> 
struct meta_type< ::std::pair<FirstType, SecondType> > 
: detail::static_template_name<
	BOOST_MIRRORED_NAMESPACE(::std),
	detail::meta_type_std_pair<FirstType, SecondType> , 
	mpl::vector2<FirstType, SecondType>
>
{
	typedef ::std::pair<FirstType, SecondType> reflected_type;
};


} // namespace mirror
} // namespace boost

#endif //include guard

