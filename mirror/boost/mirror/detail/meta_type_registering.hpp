/**
 * \file boost/mirror/detail/meta_type_registering.hpp
 *
 * Registering of types
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_DETAIL_META_TYPE_REGISTERING_HPP
#define BOOST_MIRROR_DETAIL_META_TYPE_REGISTERING_HPP

// forward declarations
#include <boost/mirror/meta_data_fwd.hpp>
// meta namespaces 
#include <boost/mirror/meta_namespace.hpp>

namespace boost {
namespace mirror {

/** Meta-data describing types 
 */
template <typename Type>
struct meta_type 
: public detail::full_name_builder<
	typename detail::registered_type_info<Type>::scope,
	detail::registered_type_info<Type>
>
{
	typedef detail::full_name_builder<
        	typename detail::registered_type_info<Type>::scope,
        	detail::registered_type_info<Type>
	> base_class;

	inline static const bstring& base_name(void)
	{
		return base_class::get_name(mpl::false_());
	}

	inline static const bstring& full_name(void)
	{
		return base_class::get_name(mpl::true_());
	}

	typedef detail::registered_type_info<Type> base_info;
	typedef typename base_info::scope scope;
	typedef typename base_info::reflected_type reflected_type;
};

#define BOOST_MIRROR_REG_TYPE_DECLARE_BASE_NAME(TYPE_NAME_STRING) \
	static const bstring& get_name(mpl::false_) \
	{ \
		static bstring s_name(BOOST_STR_LIT(TYPE_NAME_STRING)); \
		return s_name; \
	} 


/** Helper macro used to declare base-name getting functions
 *  and base-name length static constants
 */
#define BOOST_MIRROR_REG_TYPE_DECLARE_BASE_NAME_HELPER(TYPE_NAME) \
	BOOST_MIRROR_REG_TYPE_DECLARE_BASE_NAME(#TYPE_NAME)

/** Macro for registering global-scope types
 */
#define BOOST_MIRROR_REG_TYPE_GLOBAL_SCOPE(BASE_NAME) \
	namespace detail { \
	template <> struct registered_type_info< BASE_NAME > \
	{ \
		typedef BOOST_MIRRORED_GLOBAL_SCOPE() scope; \
		typedef BASE_NAME reflected_type; \
		BOOST_MIRROR_REG_TYPE_DECLARE_BASE_NAME_HELPER(BASE_NAME) \
	}; \
	} // namespace detail

/** 'Shorthand' for BOOST_MIRROR_REG_TYPE_GLOBAL_SCOPE macro 
 */
#define BOOST_MIRROR_REG_TYPE_GS(BASE_NAME) \
	BOOST_MIRROR_REG_TYPE_GLOBAL_SCOPE(BASE_NAME)


/** Macro for registering types nested in namespaces
 */
#define BOOST_MIRROR_REG_TYPE(NAMESPACE, BASE_NAME) \
	namespace detail { \
	template <> struct registered_type_info< NAMESPACE::BASE_NAME > \
	{ \
		typedef BOOST_MIRRORED_NAMESPACE(NAMESPACE) scope; \
		typedef NAMESPACE::BASE_NAME reflected_type; \
		BOOST_MIRROR_REG_TYPE_DECLARE_BASE_NAME_HELPER(BASE_NAME) \
	}; \
	} // namespace detail


/** All typedefined types are registered in this namespace
 *  as templates
 */
namespace typedef_ {

	// this type is used to recognize the Typedef<Namespace, typedef_tag>
	// templates representing registered typedefs from other
	// 'template <class, class> class' templates
	struct typedef_tag { };

	/** this meta-function is used to extract the type 
	 *  from regular types and typedef type selectors
	 * in a uniform way.
	 * 
	 * By default the template argument is 'returned'
	 */
	template <class TypeOrTypedefSelector>
	struct extract_type
	{
		typedef TypeOrTypedefSelector type;
	};

	/** This is specialization for the templates in mirror::typedef_
	 *  that represent the registered typedefined types.
	 *  In this case the base_type of the selector is returned
	 */
	template <
		template <class, class> class TypedefSelector, 
		class NamespacePlaceholder
	>
	struct extract_type<TypedefSelector<NamespacePlaceholder, typedef_tag> >
	{
		typedef typename TypedefSelector<
			NamespacePlaceholder, 
			typedef_tag
		>::base_type type;
	};
} // namespace typedef_



/** Macro that expands into a typedef-ined type selector
 */
#define BOOST_MIRROR_GET_TYPEDEFD_TYPE_SELECTOR(NAMESPACE, TYPEDEFD_NAME) \
	::boost::mirror::typedef_::TYPEDEFD_NAME < \
		BOOST_MIRRORED_NAMESPACE( NAMESPACE ), \
		::boost::mirror::typedef_::typedef_tag \
	> 

/** Macro that expands into a typedef-ined type selector
 */
#define BOOST_MIRROR_GET_TYPEDEFD_TYPE_SELECTOR_GS(TYPEDEFD_NAME) \
	::boost::mirror::typedef_::TYPEDEFD_NAME < \
		BOOST_MIRRORED_GLOBAL_SCOPE(), \
		::boost::mirror::typedef_::typedef_tag \
	>

#define BOOST_MIRROR_TYPEDEF(NAMESPACE, TYPEDEFD_NAME) \
	BOOST_MIRROR_GET_TYPEDEFD_TYPE_SELECTOR(NAMESPACE, TYPEDEFD_NAME)

#define BOOST_MIRROR_TYPEDEF_GS(TYPEDEFD_NAME) \
	BOOST_MIRROR_GET_TYPEDEFD_TYPE_SELECTOR_GS(TYPEDEFD_NAME)

/** Macro for registering typedef-ined types in the global scope
 */
#define BOOST_MIRROR_REG_TYPEDEF_GLOBAL_SCOPE(TYPEDEFD_NAME) \
	namespace typedef_ { \
		template <class MetaNamespace, class Tag> struct TYPEDEFD_NAME; \
		template <> struct TYPEDEFD_NAME< \
			BOOST_MIRRORED_GLOBAL_SCOPE(), \
			::boost::mirror::typedef_::typedef_tag \
		> {typedef TYPEDEFD_NAME base_type;}; \
	} /* namespace typedef_ */ \
	namespace detail { \
	template <> struct registered_type_info< \
		BOOST_MIRROR_GET_TYPEDEFD_TYPE_SELECTOR_GS(TYPEDEFD_NAME) \
	> \
	{ \
		typedef BOOST_MIRRORED_GLOBAL_SCOPE() scope; \
		typedef ::TYPEDEFD_NAME reflected_type; \
		BOOST_MIRROR_REG_TYPE_DECLARE_BASE_NAME_HELPER(TYPEDEFD_NAME) \
	}; \
	} // namespace detail

#define BOOST_MIRROR_REG_TYPEDEF_GS(TYPEDEFD_NAME) \
	BOOST_MIRROR_REG_TYPEDEF_GLOBAL_SCOPE(TYPEDEFD_NAME) 


/** Macro for registering typedef-ined types in namespaces
 */
#define BOOST_MIRROR_REG_TYPEDEF(NAMESPACE, TYPEDEFD_NAME) \
	namespace typedef_ { \
		template <class MetaNamespace, class Tag> struct TYPEDEFD_NAME; \
		template <> struct TYPEDEFD_NAME< \
			BOOST_MIRRORED_NAMESPACE( NAMESPACE ), \
			::boost::mirror::typedef_::typedef_tag \
		> {typedef NAMESPACE :: TYPEDEFD_NAME base_type;}; \
	} /* namespace typedef_ */ \
	namespace detail { \
	template <> struct registered_type_info< \
		BOOST_MIRROR_GET_TYPEDEFD_TYPE_SELECTOR(NAMESPACE, TYPEDEFD_NAME) \
	> \
	{ \
		typedef BOOST_MIRRORED_NAMESPACE(NAMESPACE) scope; \
		typedef NAMESPACE::TYPEDEFD_NAME reflected_type; \
		BOOST_MIRROR_REG_TYPE_DECLARE_BASE_NAME_HELPER(TYPEDEFD_NAME) \
	}; \
	} // namespace detail

/** Declaration of meta types for types in declared inside
 *  of a class.
 */
#define BOOST_MIRROR_REG_TYPE_EMBEDDED(WRAPPER, BASE_NAME) \
	namespace detail { \
	template <> struct registered_type_info< WRAPPER::BASE_NAME > \
	{ \
		typedef meta_type< WRAPPER > scope; \
		typedef WRAPPER::BASE_NAME reflected_type; \
		BOOST_MIRROR_REG_TYPE_DECLARE_BASE_NAME_HELPER(BASE_NAME) \
	}; \
	} // namespace detail

#define BOOST_MIRROR_REG_TYPE_EMB(WRAPPER, BASE_NAME) \
	BOOST_MIRROR_REG_TYPE_EMBEDDED(WRAPPER, BASE_NAME)


} // namespace mirror
} // namespace boost

#endif //include guard

