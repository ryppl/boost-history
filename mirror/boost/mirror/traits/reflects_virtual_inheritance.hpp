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
 #include  <boost/type_traits/is_same.hpp>
#include <boost/mirror/meta_inheritance.hpp>

namespace boost {
namespace mirror {

template <class MetaObject>
struct reflects_virtual_inheritance : public false_type{ };

template < 
        class DerivedClass,
        class VariantTag,
        class Position,
        class BaseClass,
        class AccessSpecifier,
	class InheritanceSpecifier
>
struct reflects_virtual_inheritance<
	detail::class_inheritance_info<
		DerivedClass,
		VariantTag,
		Position,
		BaseClass,
		AccessSpecifier,
		InheritanceSpecifier
	> 
> : public is_same< InheritanceSpecifier, virtual_base_ > { };

template < class InheritanceInfo >
struct reflects_virtual_inheritance<
	meta_inheritance< InheritanceInfo > 
> : public reflects_virtual_inheritance<InheritanceInfo>{ };


} // namespace mirror
} // namespace boost

#endif //include guard

