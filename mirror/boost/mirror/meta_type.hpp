/**
 * \file boost/mirror/meta_type.hpp
 * Registering and reflection of types
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_TYPE_HPP
#define BOOST_MIRROR_META_TYPE_HPP

// meta namespaces (includes boost/char_type_switch/string.hpp)
#include <boost/mirror/meta_namespace.hpp>
// forward declarations
#include <boost/mirror/meta_data_fwd.hpp>
// for declarations of meta-types for groups of types
#include <boost/preprocessor.hpp>
//
//
#include <boost/mirror/detail/const_type_name.hpp>
#include <boost/mirror/detail/volatile_type_name.hpp>
#include <boost/mirror/detail/cv_type_name.hpp>

#include <boost/mirror/detail/pointer_type_name.hpp>
#include <boost/mirror/detail/reference_type_name.hpp>
#include <boost/mirror/detail/array_type_name.hpp>

namespace boost {
namespace mirror {


namespace detail {

template <class type_identifier, typename base_type>
struct typedefd_type_selector { };

} // namespace detail

/** Macro that expands into a typedefined type selector
 */
#define BOOST_MIRROR_TYPEDEFD_SELECTOR(IDENTIFIER, BASE_TYPE)\
	::boost::mirror::detail::typedefd_type_selector< ::boost::mirror::typedefs::IDENTIFIER, BASE_TYPE >

template <class type_identifier, typename base_type>
struct meta_type< ::boost::mirror::detail::typedefd_type_selector<
	type_identifier, base_type
> > : meta_type<base_type>{ };

/** Helper macro that declared the full_name-related stuff
 */
#define BOOST_MIRROR_TMP_DECLARE_META_TYPE_FULL_NAME() \
	BOOST_MIRROR_CONST_MEMBER_ATTRIB(\
		size_t, \
		full_name_length, \
		scope::full_name_length + 2 + \
		base_name_length \
	) \
	static const bchar* full_name(void)\
	{\
		static bchar the_full_name[full_name_length] = \
			BOOST_STR_LIT(""); \
		if(!the_full_name[0])  \
		{ \
			bchar * pos = the_full_name; \
			bstrncpy(pos, scope::full_name(), scope::full_name_length);\
			pos += scope::full_name_length; \
			bstrncpy(pos, BOOST_STR_LIT("::"), 2);\
			pos += 2; \
			bstrncpy(pos, base_name(), base_name_length);\
		} \
		return the_full_name; \
	}


/** Macro for declaration of meta-types
 */
#define BOOST_MIRROR_REG_META_TYPE(NAMESPACE_ALIAS, NAMESPACE, BASE_NAME)     \
	template <> struct meta_type< NAMESPACE::BASE_NAME >              \
	{                                                                 \
		typedef BOOST_MIRROR_REFLECT_NAMESPACE(NAMESPACE_ALIAS) scope;                        \
		typedef NAMESPACE::BASE_NAME base_type;                                  \
		static const bchar* base_name(void) {return BOOST_STR_LIT(#BASE_NAME);}\
		BOOST_MIRROR_CONST_MEMBER_ATTRIB( \
			size_t, \
			base_name_length, \
			BOOST_STR_LIT_LENGTH(#BASE_NAME)\
		) \
		BOOST_MIRROR_TMP_DECLARE_META_TYPE_FULL_NAME() \
	};

/** Macro for declaration of meta-types for typedefined types
 */
#define BOOST_MIRROR_REG_META_TYPEDEFD(NAMESPACE_ALIAS, NAMESPACE, TYPEDEFD_NAME)     \
	namespace typedefs { struct NAMESPACE_ALIAS##_##TYPEDEFD_NAME { }; }\
	template <> struct meta_type< BOOST_MIRROR_TYPEDEFD_SELECTOR(\
		NAMESPACE_ALIAS##_##TYPEDEFD_NAME, \
		NAMESPACE::TYPEDEFD_NAME \
	) >              \
	{                                                                 \
		typedef BOOST_MIRROR_REFLECT_NAMESPACE(NAMESPACE_ALIAS) scope;                        \
		typedef NAMESPACE::TYPEDEFD_NAME base_type;                                  \
		static const bchar* base_name(void) {return BOOST_STR_LIT(#TYPEDEFD_NAME);}\
		BOOST_MIRROR_CONST_MEMBER_ATTRIB( \
			size_t, \
			base_name_length, \
			BOOST_STR_LIT_LENGTH(#TYPEDEFD_NAME)\
		) \
		BOOST_MIRROR_TMP_DECLARE_META_TYPE_FULL_NAME() \
	};

/** Declaration of meta types for types in the global scope
 */
#define BOOST_MIRROR_REG_META_TYPE_GLOBAL_SCOPE(BASE_NAME)   \
	template <> struct meta_type< BASE_NAME >              \
	{                                                                 \
		typedef BOOST_MIRROR_REFLECT_NAMESPACE(_) scope;                        \
		typedef BASE_NAME base_type;                                  \
		static const bchar* base_name(void) {return BOOST_STR_LIT(#BASE_NAME);}\
		BOOST_MIRROR_CONST_MEMBER_ATTRIB( \
			size_t, \
			base_name_length, \
			BOOST_STR_LIT_LENGTH( #BASE_NAME ) \
		) \
		static const bchar* full_name(void) {return base_name();}\
		BOOST_MIRROR_CONST_MEMBER_ATTRIB(size_t, full_name_length, base_name_length) \
	};

/** Declaration of meta types for types in declared inside
 *  of a class.
 */
#define BOOST_MIRROR_REG_META_TYPE_EMBEDDED(WRAPPER, BASE_NAME)   \
	template <> struct meta_type< WRAPPER::BASE_NAME >              \
	{                                                                 \
		typedef meta_class< WRAPPER > scope;                        \
		typedef WRAPPER::BASE_NAME base_type;                                  \
		static const bchar* base_name(void) {return BOOST_STR_LIT(#BASE_NAME);}\
		BOOST_MIRROR_CONST_MEMBER_ATTRIB( \
			size_t, \
			base_name_length, \
			BOOST_STR_LIT_LENGTH(#BASE_NAME)\
		) \
		BOOST_MIRROR_TMP_DECLARE_META_TYPE_FULL_NAME() \
	};


/** Helper macro used for batch registering of the meta-types for
 *  the C++ native types
 */
#define BOOST_MIRROR_REG_ITH_META_TYPE_NATIVE(I, _, BASE_NAME)\
	BOOST_MIRROR_REG_META_TYPE_GLOBAL_SCOPE(BASE_NAME)

#define BOOST_MIRROR_NATIVE_TYPES \
	BOOST_PP_TUPLE_TO_LIST( \
		15, \
		( \
			void, \
			bool, \
			char, signed char, unsigned char, wchar_t, \
			unsigned short int, short, \
			int, unsigned int, \
			long, unsigned long int, \
			float, \
			double, long double \
		) \
	)

/** Declare the meta types for the native C++ types
 */
BOOST_PP_LIST_FOR_EACH(BOOST_MIRROR_REG_ITH_META_TYPE_NATIVE, _, BOOST_MIRROR_NATIVE_TYPES)

/** We're done with registering of the meta types for native 
 *  types now so we don't need this anymore
 */
#undef BOOST_MIRROR_REG_ITH_META_TYPE_NATIVE


/** Register std string and wstring
 */
BOOST_MIRROR_REG_META_TYPE(_std, ::std, string)
BOOST_MIRROR_REG_META_TYPE(_std, ::std, wstring)
/** Now register the bchar and bstring too
 */
BOOST_MIRROR_REG_META_TYPEDEFD(_boost, ::boost, bchar)
BOOST_MIRROR_REG_META_TYPEDEFD(_boost, ::boost, bstring)


/** Meta-types for pointers
 */
template <class pointee_type>
struct meta_type<pointee_type*> : detail::static_pointer_type_name<
	meta_type<pointee_type>
>
{
	typedef typename meta_type<pointee_type>::scope scope;
	typedef pointee_type* base_type; 
};

/** Meta-types for arrays
 */
template <class element_type, size_t size>
struct meta_type<element_type[size]> : detail::static_array_type_name<
	meta_type<element_type>, size
>
{
	typedef typename meta_type<element_type>::scope scope;
	typedef element_type base_type[size];
};

/** Meta-types for references
 */
template <class refered_to_type>
struct meta_type<refered_to_type&> : detail::static_reference_type_name<
	meta_type<refered_to_type>
>
{
	typedef typename meta_type<refered_to_type>::scope scope;
	typedef refered_to_type& base_type; 
};

/** Meta-types for const types
 */
template <class non_const_type>
struct meta_type<const non_const_type> : detail::static_const_type_name<
	meta_type<non_const_type>
>
{
	typedef typename meta_type<non_const_type>::scope scope;
	typedef const non_const_type base_type; 
};

/** Meta-types for volatile types
 */
template <class non_volatile_type>
struct meta_type<volatile non_volatile_type> : detail::static_volatile_type_name<
	meta_type<non_volatile_type>
>
{
	typedef typename meta_type<non_volatile_type>::scope scope;
	typedef volatile non_volatile_type base_type; 
};

/** Meta-types for const volatile types
 */
template <class non_cv_type>
struct meta_type<const volatile non_cv_type> : detail::static_cv_type_name<
	meta_type<non_cv_type>
>
{
	typedef typename meta_type<non_cv_type>::scope scope;
	typedef const volatile non_cv_type base_type; 
};


} // namespace mirror
} // namespace boost

#endif //include guard

