/**
 * \file boost/mirror/meta_types/boost/int_.hpp
 *
 * Meta-type for boost::int_<T>
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_TYPES_BOOST_OPTIONAL_HPP
#define BOOST_MIRROR_META_TYPES_BOOST_OPTIONAL_HPP

#include <boost/mirror/meta_type.hpp>
#include <boost/mirror/meta_namespaces/boost/mpl.hpp>
#include <boost/mirror/detail/template_name.hpp>
#include <boost/mpl/int.hpp> 

namespace boost {
namespace mirror {

namespace detail {

struct meta_type_boost_mpl_int_
{
	BOOST_MIRROR_REG_TYPE_DECLARE_BASE_NAME("int_")
};

} // namespace detail


template <int I> 
struct meta_type< ::boost::mpl::int_<I> > 
: detail::static_template_name<
	BOOST_MIRRORED_NAMESPACE(::boost::mpl_),
	detail::meta_type_boost_mpl_int_ , 
	mpl::vector1< ::boost::mpl::int_<I> >
>
{
	typedef ::boost::mpl::int_<I> reflected_type;
};


} // namespace mirror
} // namespace boost

#endif //include guard

