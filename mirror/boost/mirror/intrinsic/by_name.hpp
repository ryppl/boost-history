/**
 * \file boost/mirror/intrinsic/by_name.hpp
 * 
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_INTRINSIC_BY_NAME_HPP
#define BOOST_MIRROR_INTRINSIC_BY_NAME_HPP

#include <boost/mirror/meta_attributes.hpp>
#include <boost/mirror/detail/class_generators.hpp>

namespace boost {
namespace mirror {
namespace detail {


template <class MetaClassAttributes>
struct attrib_by_name : public class_generator<
	MetaClassAttributes,
	get_meta_attrib_generator_plugin,
	mpl::true_
> { };


} // namespace detail

template <class MetaObjectSequence>
struct by_name : detail::attrib_by_name<MetaObjectSequence>
{ };

} // namespace mirror
} // namespace boost

#endif //include guard

