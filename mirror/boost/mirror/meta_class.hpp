/**
 * \file boost/mirror/meta_class.hpp
 * Registering and reflection of classes
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_CLASS_HPP
#define BOOST_MIRROR_META_CLASS_HPP

#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/comparison.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/remove_if.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/insert_range.hpp>
#include <boost/mpl/joint_view.hpp>
//
// basic meta types
#include <boost/mirror/meta_type.hpp>
// reflection of class inheritance
#include <boost/mirror/meta_inheritance.hpp>
// inheritance trait
#include <boost/mirror/traits/reflects_virtual_inheritance.hpp>
// reflection of class member attributes
#include <boost/mirror/meta_attributes.hpp>
//
// reflection of destructors
#include <boost/mirror/meta_destructor.hpp>

namespace boost {
namespace mirror {
namespace detail {

/** Common base for various meta_class specializations 
 */
template <
	class Class, 
	class VariantTag
>
struct meta_class_base
{
	/** The base classes of a class.
	 *  The member base_classes::list is a mpl::vector of 
	 *  meta_inheritance<> specializations, one for every 
	 *  base class.
	 */
	typedef meta_base_classes<Class, VariantTag> base_classes;

	/** The member attributes of the class (not including the inherited
	 *  member attribs. 
	 *  The attributes::type_list is a mpl::vector of types 
	 *  of the attributes.
	 *
	 *  The attributes::get(mpl::int_<I>) functions allow to get the value
	 *  of the I-th attribute, if the attribute is not write-only, where:
	 *  0 <= I < N; N = $mpl::size<attributes::type_list>::value
	 *
	 * The attributes::set(mpl::int_<I>), T val) functions allow to set
	 * the value of the I-th attribute, if it's not read-only, where
	 *  0 <= I < N; N = $mpl::size<attributes::type_list>::value
	 * 
	 */
	typedef meta_class_attributes<Class, VariantTag> 
		attributes;

	/** Same as attributes but containing also the inherited attributes
	 */
	typedef meta_class_all_attributes<Class, VariantTag >
		all_attributes;

	/** Meta data concerning the destructor of the reflected class
	 */
	typedef meta_class_destructor<Class, VariantTag>
		destructor;
};

} // namespace detail

/** Meta class - specializes the meta_type for classes
 */
template <
	class Class, 
	class VariantTag
>
struct meta_class
: public meta_type<Class>
, public detail::meta_class_base<Class, VariantTag>
{ };

/** Meta class - specialization for typedefined types
 */
template <
	template <class> class TypedefSelector, 
	class MetaNamespace,
	class VariantTag
>
struct meta_class<
	TypedefSelector<typedef_::typedef_tag<MetaNamespace> >,
	VariantTag
>
: public meta_type<
	TypedefSelector<typedef_::typedef_tag<MetaNamespace> >
>
, public detail::meta_class_base<
	typename typedef_::extract_type<
		TypedefSelector<
			typedef_::typedef_tag<MetaNamespace> 
		> 
	>::type, 
	VariantTag
>
{ };




/** This macro should be included in the definition of every class
 *  with private or protected members, that should be refleccted
 */
#define BOOST_MIRROR_FRIENDLY_CLASS(CLASS_NAME) \
		friend struct ::boost::mirror::meta_class_attributes<CLASS_NAME>;



} // namespace mirror
} // namespace boost

#endif //include guard

