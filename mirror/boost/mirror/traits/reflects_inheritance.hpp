/**
 * \file boost/mirror/reflects_inheritance.hpp
 * Meta function that returns true if the given template argument
 * is an instantiation of meta_inheritance
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_REFLECTS_INHERITANCE_HPP
#define BOOST_MIRROR_META_REFLECTS_INHERITANCE_HPP

// true type/false type for trait templates 
#include <boost/type_traits/integral_constant.hpp>
#include <boost/mirror/meta_inheritance.hpp>

namespace boost {
namespace mirror {

template <class MetaObject>
struct reflects_inheritance : public false_type { };

template <
	class Position,
	class BaseClass,
	class AccessSpec, 
	class VirtualSpec
>
struct reflects_inheritance<
	meta_inheritance<
		Position,
		BaseClass,
		AccessSpec,
		VirtualSpec
	> 
> : public true_type{ };


} // namespace mirror
} // namespace boost

#endif //include guard

