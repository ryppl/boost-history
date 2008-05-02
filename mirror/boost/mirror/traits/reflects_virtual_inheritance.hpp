/**
 * \file boost/mirror/reflects_virtual_inheritance.hpp
 * Meta function that returns true if the given meta-inheritance
 * reflects virtual inheritance.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_REFLECTS_VIRTUAL_INHERITANCE_HPP
#define BOOST_MIRROR_META_REFLECTS_VIRTUAL_INHERITANCE_HPP

// true type/false type for trait templates 
#include <boost/type_traits/integral_constant.hpp>
#include <boost/mirror/meta_inheritance.hpp>

namespace boost {
namespace mirror {

template <class MetaObject>
struct reflects_virtual_inheritance;

template <
	class Position,
	class BaseClass,
	typename AccessSpec 
>
struct reflects_virtual_inheritance<
	meta_inheritance<
		Position,
		BaseClass,
		AccessSpec,
		virtual_base_
	> 
> : public true_type{ };

template <
	class Position,
	class BaseClass,
	typename AccessSpec 
>
struct reflects_virtual_inheritance<
	meta_inheritance<
		Position,
		BaseClass,
		AccessSpec,
		nonvirtual_base_
	> 
> : public false_type{ };


} // namespace mirror
} // namespace boost

#endif //include guard

