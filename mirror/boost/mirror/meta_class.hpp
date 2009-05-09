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
// preprocessor
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
// basic meta types
#include <boost/mirror/meta_type.hpp>
// reflection of class inheritance
#include <boost/mirror/meta_inheritance.hpp>
// inheritance trait
#include <boost/mirror/traits/reflects_virtual_inheritance.hpp>
// reflection of class member attributes
#include <boost/mirror/meta_attributes.hpp>
//
#include <boost/mirror/meta_constructors.hpp>
#include <boost/mirror/meta_mem_functions.hpp>
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

	typedef meta_constructors<Class, VariantTag>
		constructors;

	typedef meta_member_functions<Class, VariantTag>
		member_functions;
	
	/** Meta data concerning the destructor of the reflected class
	 *  EXPERIMENTAL
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
	friend struct ::boost::mirror::meta_class_attributes_base<CLASS_NAME>;


/** A helper macro used in the BOOST_MIRROR_QREG_* macros,
 *  as a parameter to the BOOST_PP_SEQ_FOR_EACH_I and
 *  registers the i-th base class of the registered class
 */
#define BOOST_MIRROR_QREG_CLASS_REG_BASE_CLASS_HELPER(R, DATA, I, BASE_CLASS)\
	BOOST_MIRROR_REG_BASE_CLASS(I, public, BASE_CLASS)

/** A helper macro used in the BOOST_MIRROR_QREG_* macros,
 *  as a parameter to the BOOST_PP_SEQ_FOR_EACH_I and
 *  registers the i-th virtual base class of the registered class
 */
#define BOOST_MIRROR_QREG_CLASS_REG_V_BASE_CLASS_HELPER(R, DATA, I, BASE_CLASS)\
	BOOST_MIRROR_REG_VIRTUAL_BASE_CLASS(I, public, BASE_CLASS)

/** Another helper macro used in the BOOST_MIRROR_QREG_* macros
 *  as a parameter to BOOST_PP_SEQ_FOR_EACH, which registers
 *  a class member attribute
 */
#define BOOST_MIRROR_QREG_CLASS_REG_ATTRIB_HELPER(R, DATA, ATTRIB)\
	BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, ATTRIB)


/** This macro provides a quick all-in-one registering of
 *  a class. It registers the class' type, its base classes
 *  and member attributes. 
 */
#define BOOST_MIRROR_QREG_CLASS( \
	NAMESPACE, \
	CLASS, \
	BASE_CLASS_PP_SEQ, \
	ATTRIB_PP_SEQ \
) \
/** Registers the type */ \
BOOST_MIRROR_REG_TYPE(NAMESPACE, CLASS) \
/** Registers the base classes */ \
BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(NAMESPACE :: CLASS) \
BOOST_PP_SEQ_FOR_EACH_I( \
	BOOST_MIRROR_QREG_CLASS_REG_BASE_CLASS_HELPER, \
	_, \
	BASE_CLASS_PP_SEQ \
) \
BOOST_MIRROR_REG_BASE_CLASSES_END \
/** Registers the class' attributes */ \
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(NAMESPACE :: CLASS) \
BOOST_PP_SEQ_FOR_EACH( \
	BOOST_MIRROR_QREG_CLASS_REG_ATTRIB_HELPER, \
	_, \
	ATTRIB_PP_SEQ \
) \
BOOST_MIRROR_REG_CLASS_ATTRIBS_END


/** A variant of BOOST_MIRROR_QREG_CLASS for classes that have 
 *  only virtual base classes (i.e. no base clases with regular
 *  inheritance).
 */
#define BOOST_MIRROR_QREG_CLASS_V_BASES( \
	NAMESPACE, \
	CLASS, \
	BASE_CLASS_PP_SEQ, \
	ATTRIB_PP_SEQ \
) \
/** Registers the type */ \
BOOST_MIRROR_REG_TYPE(NAMESPACE, CLASS) \
/** Registers the base classes */ \
BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(NAMESPACE :: CLASS) \
BOOST_PP_SEQ_FOR_EACH_I( \
	BOOST_MIRROR_QREG_CLASS_REG_V_BASE_CLASS_HELPER, \
	_, \
	BASE_CLASS_PP_SEQ \
) \
BOOST_MIRROR_REG_BASE_CLASSES_END \
/** Registers the class' attributes */ \
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(NAMESPACE :: CLASS) \
BOOST_PP_SEQ_FOR_EACH( \
	BOOST_MIRROR_QREG_CLASS_REG_ATTRIB_HELPER, \
	_, \
	ATTRIB_PP_SEQ \
) \
BOOST_MIRROR_REG_CLASS_ATTRIBS_END


/** Similar to BOOST_MIRROR_QREG_CLASS this macro registers
 *  a class without base classes. 
 */
#define BOOST_MIRROR_QREG_CLASS_NO_BASE( \
        NAMESPACE, \
        CLASS, \
        ATTRIB_PP_SEQ \
) \
/** Registers the type */ \
BOOST_MIRROR_REG_TYPE(NAMESPACE, CLASS) \
/** Registers the class' attributes */ \
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(NAMESPACE :: CLASS) \
BOOST_PP_SEQ_FOR_EACH( \
        BOOST_MIRROR_QREG_CLASS_REG_ATTRIB_HELPER, \
        _, \
        ATTRIB_PP_SEQ \
) \
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

/** similar to BOOST_MIRROR_QREG_CLASS this macro registers
 *  a class with base classes but without any own attributes
 */
#define BOOST_MIRROR_QREG_CLASS_NO_ATTR( \
        NAMESPACE, \
        CLASS, \
        BASE_CLASS_PP_SEQ, \
        ATTRIB_PP_SEQ \
) \
/** Registers the type */ \
BOOST_MIRROR_REG_TYPE(NAMESPACE, CLASS) \
/** Registers the base classes */ \
BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(NAMESPACE :: CLASS) \
BOOST_PP_SEQ_FOR_EACH_I( \
        BOOST_MIRROR_QREG_CLASS_REG_BASE_CLASS_HELPER, \
        _, \
        BASE_CLASS_PP_SEQ \
) \
BOOST_MIRROR_REG_BASE_CLASSES_END \

} // namespace mirror
} // namespace boost

#endif //include guard

