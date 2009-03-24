/**
 * \file boost/mirror/detail/meta_attrib_traits.hpp
 *  
 *  Additional traits of meta-attributes
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_DETAIL_META_ATTRIB_TRAITS_HPP
#define BOOST_MIRROR_DETAIL_META_ATTRIB_TRAITS_HPP

namespace boost {
namespace mirror {

/** Possible class attribute storage specifiers 
 */
namespace attrib_storage_specifiers {
	// static class attribute 
	struct static_ { };
	// mutable class attribute
	struct mutable_ { };
	// regular class attribure
	struct __ { };
}

/** Additional attribute traits containing information
 *  about storage class specifiers and some information 
 *  about the type of the attribute
 */
template <
	class Specifiers, 
	class TypeOrTypedefSelector
> struct meta_class_attribute_traits;

/** Specialization for non-static, non-mutable members
 */
template <class TypeOrTypedefSelector>
struct meta_class_attribute_traits<
	attrib_storage_specifiers::__,
	TypeOrTypedefSelector
>
{
	typedef mpl::false_ is_static;
	typedef mpl::false_ is_mutable;
	typedef TypeOrTypedefSelector meta_type_selector;
};

/** Specialization for static member attribs
 */
template <class TypeOrTypedefSelector>
struct meta_class_attribute_traits<
	attrib_storage_specifiers::static_,
	TypeOrTypedefSelector
>
{
	typedef mpl::true_ is_static;
	typedef mpl::false_ is_mutable;
	typedef TypeOrTypedefSelector meta_type_selector;
};

/** Specialization for mutable member attribs
 */
template <class TypeOrTypedefSelector>
struct meta_class_attribute_traits<
	attrib_storage_specifiers::mutable_,
	TypeOrTypedefSelector
>
{
	typedef mpl::false_ is_static;
	typedef mpl::true_ is_mutable;
	typedef TypeOrTypedefSelector meta_type_selector;
};

} // namespace mirror
} // namespace boost

#endif //include guard

