/**
 * \file boost/mirror/meta_inheritance.hpp
 * Templates used in describing class inheritance
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_INHERITANCE_HPP
#define BOOST_MIRROR_META_INHERITANCE_HPP

// forward declarations
#include <boost/mirror/meta_data_fwd.hpp>
// access specifiers
#include <boost/mirror/access_spec.hpp>
// inheritance specifiers
#include <boost/mirror/inherit_spec.hpp>
// class 'kind' specifiers
#include <boost/mirror/class_kind_spec.hpp>
//
#include <boost/mpl/vector.hpp>

namespace boost {
namespace mirror {
namespace detail {

/** Internal information about a class inheritance 
 */
template <
	class DerivedClass,
	class VariantTag,
	class Position,
	class BaseClass,
	class AccessSpecifier,
	class InheritanceSpecifier
>
struct class_inheritance_info
{
	struct detail {
		typedef DerivedClass derived_class;
		typedef VariantTag derived_class_vt;
	}; // struct detail

	// the scope ot the meta_inheritance
	typedef BOOST_MIRRORED_CLASS(DerivedClass) scope;
	// the position inside of the containder
	typedef Position position;
	// access specifier
	typedef AccessSpecifier access;
	// inheritance specifier
	typedef InheritanceSpecifier inheritance;
	// meta_class reflecting the base class
	typedef BOOST_MIRRORED_CLASS(BaseClass) base_class;
};


} // namespace detail


/** Forward declaration of the meta_base_classes template 
 */
template <
	class Class, 
	class VariantTag = detail::default_meta_class_variant
>
struct meta_base_classes;

/** This template stores the inheritance type and access specifier
 *  of a base class for a derived class
 */
template <
	class InheritanceInfo
>
struct meta_inheritance : public InheritanceInfo
{
	// the container where this meta inheritance belongs
	typedef meta_base_classes<
		typename InheritanceInfo::detail::derived_class,
		typename InheritanceInfo::detail::derived_class_vt
	> container;
	typedef typename InheritanceInfo::position position;
	typedef typename InheritanceInfo::scope scope;
	typedef typename InheritanceInfo::inheritance inheritance;
	typedef typename InheritanceInfo::access access;
	typedef typename InheritanceInfo::base_class base_class;
};

/** Default (empty) list of base classes of a meta_class
 */
template <
	class Class, 
	class VariantTag
>
struct meta_base_classes
{
	typedef BOOST_MIRRORED_CLASS(Class) scope;
	typedef mpl::vector0<> list;
};


/** This macro starts the declaration of base classes 
 *  of the given class
 */
#define BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(A_CLASS) \
	template <> struct meta_base_classes<\
		A_CLASS, \
		detail::default_meta_class_variant \
	> { \
		typedef BOOST_MIRRORED_CLASS(A_CLASS) scope; \
		typedef A_CLASS derived_class; \
		typedef detail::default_meta_class_variant derived_class_vt; \
		typedef mpl::vector< 

/** This macro declares that the A_BASE_CLASS class is the i-th
 *  base class of the given class, with default access specifier
 *  and inheritance type specifier. The numbering starts from 
 *  zero, declarations must be ordered by number.
 */
#define BOOST_MIRROR_REG_SIMPLE_BASE_CLASS(NUMBER, A_BASE_CLASS) \
	BOOST_PP_COMMA_IF(NUMBER) \
	detail::class_inheritance_info<\
		derived_class, \
		derived_class_vt, \
		mpl::int_<NUMBER>, \
		A_BASE_CLASS, \
		class_kind_default_access<\
			meta_class_kind< A_BASE_CLASS >::result \
		>::specifier, \
		nonvirtual_base_ \
	> 

/** This macro declares that the A_BASE_CLASS class is the i-th
 *  base class of the given class, with the given access specifier
 *  and default inheritance type specifier. The numbering starts from 
 *  zero, declarations must be ordered by number.
 */
#define BOOST_MIRROR_REG_BASE_CLASS(NUMBER, ACCESS_SPEC, A_BASE_CLASS) \
	BOOST_PP_COMMA_IF(NUMBER) \
	detail::class_inheritance_info< \
		derived_class, \
		derived_class_vt, \
		mpl::int_<NUMBER>, \
		A_BASE_CLASS, \
		ACCESS_SPEC##_, \
		nonvirtual_base_ \
	> 

/** This macro declares that the A_BASE_CLASS class is the i-th
 *  base class of the given class, with the given access specifier
 *  and virtual inheritance type specifier. The numbering starts from 
 *  zero, declarations must be ordered by number.
 */
#define BOOST_MIRROR_REG_VIRTUAL_BASE_CLASS(NUMBER, ACCESS_SPEC, A_BASE_CLASS) \
	BOOST_PP_COMMA_IF(NUMBER) \
	detail::class_inheritance_info< \
		derived_class, \
		derived_class_vt, \
		mpl::int_<NUMBER>, \
		A_BASE_CLASS, \
		ACCESS_SPEC##_, \
		virtual_base_ \
	> 

/** This macro finishes the declaration of base classes
 *  of the given class
 */
#define BOOST_MIRROR_REG_BASE_CLASSES_END \
	> list; \
};

/** This macro registers a the A_BASE_CLASS class 
 *  as the only base class of the A_CLASS class
 *  with the ACCESS_SPEC access specifier
 */
#define BOOST_MIRROR_REG_SINGLE_BASE_CLASS(A_CLASS, ACCESS_SPEC, A_BASE_CLASS) \
	BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(A_CLASS) \
	BOOST_MIRROR_REG_BASE_CLASS(0, ACCESS_SPEC, A_BASE_CLASS) \
	BOOST_MIRROR_REG_BASE_CLASSES_END

/** This macro registers a the A_BASE_CLASS class 
 *  as the only virtual base class of the A_CLASS class
 *  with the ACCESS_SPEC access specifier
 */
#define BOOST_MIRROR_REG_SINGLE_VIRTUAL_BASE_CLASS(A_CLASS, ACCESS_SPEC, A_BASE_CLASS) \
	BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(A_CLASS) \
	BOOST_MIRROR_REG_VIRTUAL_BASE_CLASS(0, ACCESS_SPEC, A_BASE_CLASS) \
	BOOST_MIRROR_REG_BASE_CLASSES_END

} // namespace mirror
} // namespace boost

#endif //include guard

