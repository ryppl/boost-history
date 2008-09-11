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


/** helper template instances of which define the inheritance type,
 *  access specifiers and base class of a meta_class
 */
template <
	class BaseClass, 
	typename AccessSpec, 
	typename InheritanceSpec
> struct meta_inheritance_defs
{
	typedef InheritanceSpec inheritance_specifier;
	typedef AccessSpec access_specifier;
	typedef BaseClass base_class;
	typedef BOOST_MIRRORED_CLASS(BaseClass) meta_base_class;
};

/** This template stores the inheritance type and access specifier
 *  of a base class for a derived class
 */
template <
	class BaseClass, 
	typename AccessSpec, 
	typename InheritanceSpec
> struct meta_inheritance_spec;

template <class BaseClass>
struct meta_inheritance_spec<
	BaseClass, 
	private_, 
	virtual_base_
> : meta_inheritance_defs<
	BaseClass, 
	private_, 
	virtual_base_
> { };

template <class BaseClass>
struct meta_inheritance_spec<
	BaseClass, 
	protected_, 
	virtual_base_
> : meta_inheritance_defs<
	BaseClass, 
	protected_, 
	virtual_base_
> { };

template <class BaseClass>
struct meta_inheritance_spec<
	BaseClass, 
	public_, 
	virtual_base_
> : meta_inheritance_defs<
	BaseClass, 
	public_, 
	virtual_base_
> { };

template <class BaseClass>
struct meta_inheritance_spec<
	BaseClass, 
	private_, 
	nonvirtual_base_
> : meta_inheritance_defs<
	BaseClass, 
	private_, 
	nonvirtual_base_
> { };

template <class BaseClass>
struct meta_inheritance_spec<
	BaseClass, 
	protected_, 
	nonvirtual_base_
> : meta_inheritance_defs<
	BaseClass, 
	protected_, 
	nonvirtual_base_
> { };

template <class BaseClass>
struct meta_inheritance_spec<
	BaseClass, 
	public_, 
	nonvirtual_base_
> : meta_inheritance_defs<
	BaseClass, 
	public_, 
	nonvirtual_base_
> { };

/** This template stores the inheritance type and access specifier
 *  of a base class for a derived class
 */
template <
	class Position,
	class BaseClass,
	typename AccessSpec, 
	typename InheritanceSpec = nonvirtual_base_
>
struct meta_inheritance
: meta_inheritance_spec<
	BaseClass, 
	AccessSpec, 
	InheritanceSpec
>
{
	typedef Position position;
};

/** Default (empty) list of base classes of a meta_class
 */
template <
	class Class, 
	class VariantTag = detail::default_meta_class_variant
>
struct meta_base_classes
{
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
		typedef mpl::vector< 

/** This macro declares that the A_BASE_CLASS class is the i-th
 *  base class of the given class, with default access specifier
 *  and inheritance type specifier. The numbering starts from 
 *  zero, declarations must be ordered by number.
 */
#define BOOST_MIRROR_REG_SIMPLE_BASE_CLASS(NUMBER, A_BASE_CLASS) \
	BOOST_PP_COMMA_IF(NUMBER) \
	meta_inheritance<\
		mpl::int_<NUMBER>, \
		A_BASE_CLASS, \
		class_kind_default_access<\
			meta_class_kind< A_BASE_CLASS >::result \
		>::specifier \
	> 

/** This macro declares that the A_BASE_CLASS class is the i-th
 *  base class of the given class, with the given access specifier
 *  and default inheritance type specifier. The numbering starts from 
 *  zero, declarations must be ordered by number.
 */
#define BOOST_MIRROR_REG_BASE_CLASS(NUMBER, ACCESS_SPEC, A_BASE_CLASS) \
	BOOST_PP_COMMA_IF(NUMBER) \
	meta_inheritance<mpl::int_<NUMBER>, A_BASE_CLASS, ACCESS_SPEC##_> 

/** This macro declares that the A_BASE_CLASS class is the i-th
 *  base class of the given class, with the given access specifier
 *  and virtual inheritance type specifier. The numbering starts from 
 *  zero, declarations must be ordered by number.
 */
#define BOOST_MIRROR_REG_VIRTUAL_BASE_CLASS(NUMBER, ACCESS_SPEC, A_BASE_CLASS) \
	BOOST_PP_COMMA_IF(NUMBER) \
	meta_inheritance<mpl::int_<NUMBER>, A_BASE_CLASS, ACCESS_SPEC##_, virtual_base_> 

/** This macro finishes the declaration of base classes
 *  of the given class
 */
#define BOOST_MIRROR_REG_BASE_CLASSES_END \
	> list; \
	struct size : public mpl::size<list>{ };\
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
#define BOOST_MIRROR_REG_SINGLE_BASE_CLASS_VIRTUAL(A_CLASS, ACCESS_SPEC, A_BASE_CLASS) \
	BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(A_CLASS) \
	BOOST_MIRROR_REG_BASE_CLASS_VIRTUAL(0, ACCESS_SPEC, A_BASE_CLASS) \
	BOOST_MIRROR_REG_BASE_CLASSES_END

} // namespace mirror
} // namespace boost

#endif //include guard

