/**
 * \file boost/mirror/meta_data_fwd.hpp
 * Forward declarations of meta-data templates
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_DATA_FWD_HPP
#define BOOST_MIRROR_META_DATA_FWD_HPP

namespace boost {
namespace mirror {

/** Meta-namespace forward template declaration
 */
template<class NamespacePlaceholder> 
struct meta_namespace;

/** Macro that expands into the meta_namespace for the 
 *  passed namespace.
 */
#define BOOST_MIRRORED_NAMESPACE(FULL_NAMESPACE_NAME) \
	::boost::mirror::meta_namespace< \
		::boost::mirror::namespace_  \
		FULL_NAMESPACE_NAME  \
		::_ \
	>
// namespace_ :: _boost_mirror_helper :: _
// namespace_ ::test_boost_mirror_helper :: _
// namespace_ ::test::foo_boost_mirror_helper :: _
// namespace_ ::test::foo::bar_boost_mirror_helper :: _

#define BOOST_MIRRORED_GLOBAL_SCOPE() \
	::boost::mirror::meta_namespace< \
		::boost::mirror::namespace_ ::_ \
	>

namespace detail {

	/** Forward declaration of the template class 
	 *  containing registered type information
	 */
	template <typename Type>
	struct registered_type_info;

} // namespace detail


/** Meta-type forward template declaration
 */
template <class Type> 
struct meta_type;


/** Macro that expands into the meta_type for the 
 *  given type or class.
 */
#define BOOST_MIRRORED_TYPE(TYPE) \
	::boost::mirror::meta_type<TYPE>

/** Macro that expands into the meta_type for the 
 *  given type or class. This is an alternate version
 *  of the BOOST_MIRRORED_TYPE taking the namespace
 *  and the type separatelly as two distinct args
 *  (thus the _NS suffix). It works however only
 *  with types defined in named namespaces.
 */
#define BOOST_MIRRORED_TYPE_NS(NAMESPACE, TYPE) \
	::boost::mirror::meta_type<NAMESPACE :: TYPE>

/** Macro that expands into the meta_type for the 
 *  given typedefined type.
 */
#define BOOST_MIRRORED_TYPEDEF(NAMESPACE, TYPEDEF) \
	::boost::mirror::meta_type< \
		BOOST_MIRROR_GET_TYPEDEFD_TYPE_SELECTOR(NAMESPACE, TYPEDEF) \
	>

/** Macro that expands into the meta_type for the 
 *  given type typedefined on the global scope.
 */
#define BOOST_MIRRORED_TYPEDEF_GLOBAL_SCOPE(TYPEDEF) \
	::boost::mirror::meta_type< \
		BOOST_MIRROR_GET_TYPEDEFD_TYPE_SELECTOR_GS( \
			TYPEDEF \
		) \
	>

#define BOOST_MIRRORED_TYPEDEF_GS(TYPEDEF) \
	BOOST_MIRRORED_TYPEDEF_GLOBAL_SCOPE(TYPEDEF) 

/** Macro that expands into the meta_type for the 
 *  type of the given expression.
 *  To get this going <boost/typeof/typeof.hpp>
 *  has to be included.
 */
#define BOOST_MIRRORED_TYPEOF(EXPRESSION) \
	::boost::mirror::meta_type<BOOST_TYPEOF(EXPRESSION)>



namespace detail {

	/** Because one class can have several different meta_classes 
	 *  we require some means to distinguish between them when
	 *  selecting the proper meta_class<> template instantiation.
	 *  This type is used to mark the default meta-class variant.
	 */ 
	struct default_meta_class_variant{ };
} //namespace detail


/** Meta-class template forward declaration
 */
template <
	class Class, 
	class VariantTag = detail::default_meta_class_variant
>
struct meta_class;

/** Macro that expands into the meta_class for the 
 *  given type or class.
 */
#define BOOST_MIRRORED_CLASS(CLASS) \
	::boost::mirror::meta_class<CLASS>

/** Macro that expands into the meta_class for the 
 *  given type or class.
 */
#define BOOST_MIRRORED_CLASS_VT(CLASS, VARIANT_TAG) \
	::boost::mirror::meta_class<\
		CLASS, \
		VARIANT_TAG\
>


/** This macro returns the meta-attribute for the ATTRIBUTE 
 *  of the CLASS. 
 *  Note that this requires mirror::at<> meta-function to be 
 *  defined.
 */
#define BOOST_MIRRORED_CLASS_ATTRIBUTE(CLASS, ATTRIBUTE) \
::boost::mirror::at< \
	BOOST_MIRRORED_CLASS(CLASS)::attributes, \
	BOOST_MIRRORED_CLASS(CLASS)::attributes::position_of_##ATTRIBUTE \
>::type


/** This is a forward declaration of the meta_constructors template
 */
template <
        class Class,
        class VariantTag = detail::default_meta_class_variant
> struct meta_constructors;

template <
        class Class ,
        class VariantTag = detail::default_meta_class_variant
> struct meta_member_functions;

#define BOOST_MIRRORED_CONSTRUCTORS(TYPE)\
	meta_constructors< TYPE >

template <
	class ReflectedType,
	class VariantTag,
	class MetaAttributes,
	class AttribPos
> struct meta_class_attribute;

template <
	class Class, 
	class VariantTag = detail::default_meta_class_variant
> struct meta_class_attributes;

template < class Class, class VariantTag>
struct meta_class_all_attributes;


} // namespace mirror
} // namespace boost

#endif //include guard
