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

// meta namespaces
#include <boost/mirror/meta_namespace.hpp>
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
	template <typename Type>
	struct defined_type_info;
} // namespace detail


/** Meta-data describing types 
 */
template <typename Type>
struct meta_type 
: public detail::full_name_builder<
	typename defined_type_info<Type>::scope,
	defined_type_info<Type>
>{ };

/** Macro for declaration of meta-types
 */
#define BOOST_MIRROR_REG_TYPE(NAMESPACE, BASE_NAME) \
	namespace detail { \
	template <> struct defined_type_info< NAMESPACE::BASE_NAME > \
	{ \
		typedef BOOST_MIRRORED_NAMESPACE(NAMESPACE) scope; \
		typedef NAMESPACE::BASE_NAME reflected_type; \
		static const bchar* base_name(void) {return BOOST_STR_LIT(#BASE_NAME);} \
		typedef typename ::boost::mpl::int_< \
			BOOST_STR_LIT_LENGTH(#BASE_NAME) \
		>::type base_name_length; \
	}; \
	} // namespace detail

/** Macro for declaration of meta-types
 */
#define BOOST_MIRROR_REG_TYPE_GLOBAL_SCOPE(BASE_NAME) \
	namespace detail { \
	template <> struct defined_type_info< BASE_NAME > \
	{ \
		typedef BOOST_MIRRORED_GLOBAL_SCOPE() scope; \
		typedef BASE_NAME reflected_type; \
		static const bchar* base_name(void) {return BOOST_STR_LIT(#BASE_NAME);} \
		typedef typename ::boost::mpl::int_< \
			BOOST_STR_LIT_LENGTH(#BASE_NAME) \
		>::type base_name_length; \
	}; \
	} // namespace detail


#ifdef NEVER_COMPILE_THIS

namespace detail {

template <class TypeIdentifier, typename Type>
struct typedefd_type_selector { };

} // namespace detail

/** Macro that expands into a typedefined type selector
 */
#define BOOST_MIRROR_TYPEDEFD_SELECTOR(IDENTIFIER, BASE_TYPE)\
	::boost::mirror::detail::typedefd_type_selector< ::boost::mirror::typedefs::IDENTIFIER, BASE_TYPE >

template <class TypeIdentifier, typename Type>
struct meta_type< ::boost::mirror::detail::typedefd_type_selector<
	TypeIdentifier, Type
> > : meta_type<Type>{ };

/** Helper macro that declared the full_name-related stuff
 */
#define BOOST_MIRROR_TMP_DECLARE_META_TYPE_FULL_NAME() \
	BOOST_STATIC_CONSTANT(\
		int, \
		full_name_length = \
		scope::full_name_length + 2 + \
		base_name_length \
	); \
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
			pos += base_name_length; \
			*pos = BOOST_STR_LIT('\0'); \
		} \
		return the_full_name; \
	}



/** Macro for declaration of meta-types for typedefined types
 */
#define BOOST_MIRROR_REG_TYPEDEFD(NAMESPACE_ALIAS, NAMESPACE, TYPEDEFD_NAME)     \
	namespace typedefs { struct NAMESPACE_ALIAS##_##TYPEDEFD_NAME { }; }\
	template <> struct meta_type< BOOST_MIRROR_TYPEDEFD_SELECTOR(\
		NAMESPACE_ALIAS##_##TYPEDEFD_NAME, \
		NAMESPACE::TYPEDEFD_NAME \
	) >              \
	{                                                                 \
		typedef BOOST_MIRROR_REFLECT_NAMESPACE(NAMESPACE_ALIAS) scope;                        \
		typedef NAMESPACE::TYPEDEFD_NAME reflected_type;                                  \
		static const bchar* base_name(void) {return BOOST_STR_LIT(#TYPEDEFD_NAME);}\
		BOOST_STATIC_CONSTANT( \
			int, \
			base_name_length = \
			BOOST_STR_LIT_LENGTH(#TYPEDEFD_NAME)\
		); \
		BOOST_MIRROR_TMP_DECLARE_META_TYPE_FULL_NAME() \
	};

/** Declaration of meta types for types in the global scope
 */
#define BOOST_MIRROR_REG_TYPE_GLOBAL_SCOPE(BASE_NAME)   \
	template <> struct meta_type< BASE_NAME >              \
	{                                                                 \
		typedef BOOST_MIRROR_REFLECT_NAMESPACE(_) scope;                        \
		typedef BASE_NAME reflected_type;                                  \
		static const bchar* base_name(void) {return BOOST_STR_LIT(#BASE_NAME);}\
		BOOST_STATIC_CONSTANT( \
			int, \
			base_name_length = \
			BOOST_STR_LIT_LENGTH( #BASE_NAME ) \
		); \
		static const bchar* full_name(void) {return base_name();}\
		BOOST_STATIC_CONSTANT(int, full_name_length = base_name_length); \
	};

/** Declaration of meta types for types in declared inside
 *  of a class.
 */
#define BOOST_MIRROR_REG_TYPE_EMBEDDED(WRAPPER, BASE_NAME)   \
	template <> struct meta_type< WRAPPER::BASE_NAME >              \
	{                                                                 \
		typedef meta_class< WRAPPER > scope;                        \
		typedef WRAPPER::BASE_NAME reflected_type;                                  \
		static const bchar* base_name(void) {return BOOST_STR_LIT(#BASE_NAME);}\
		BOOST_STATIC_CONSTANT( \
			int, \
			base_name_length = \
			BOOST_STR_LIT_LENGTH(#BASE_NAME)\
		); \
		BOOST_MIRROR_TMP_DECLARE_META_TYPE_FULL_NAME() \
	};

#endif // NEVER_COMPILE_THIS

/** Helper macro used for batch registering of the meta-types for
 *  the C++ native types
 */
#define BOOST_MIRROR_REG_ITH_META_TYPE_NATIVE(I, _, BASE_NAME)\
	BOOST_MIRROR_REG_TYPE_GLOBAL_SCOPE(BASE_NAME)

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
BOOST_MIRROR_REG_TYPE(::std, string)
BOOST_MIRROR_REG_TYPE(::std, wstring)
/** Now register the bchar and bstring too
 */
BOOST_MIRROR_REG_TYPEDEFD(::boost, bchar)
BOOST_MIRROR_REG_TYPEDEFD(::boost, bstring)


/** Meta-types for pointers
 */
template <class PointeeType>
struct meta_type<PointeeType*> : detail::static_pointer_type_name<
	meta_type<PointeeType>
>
{
	typedef typename meta_type<PointeeType>::scope scope;
	typedef PointeeType* reflected_type; 
};

/** Meta-types for arrays
 */
template <class ElementType, size_t Size>
struct meta_type<const ElementType[Size]> : detail::static_array_type_name<
	meta_type<const ElementType>, Size
>
{
	typedef typename meta_type<ElementType>::scope scope;
	typedef ElementType reflected_type[Size];
};

template <class ElementType, size_t Size>
struct meta_type<ElementType[Size]> : detail::static_array_type_name<
	meta_type<ElementType>, Size
>
{
	typedef typename meta_type<ElementType>::scope scope;
	typedef ElementType reflected_type[Size];
};

template <class ElementType>
struct meta_type<const ElementType[]> : detail::static_array_type_name<
	meta_type<const ElementType>, -1
>
{
	typedef typename meta_type<ElementType>::scope scope;
	typedef ElementType const reflected_type[];
};

template <class ElementType>
struct meta_type<ElementType[]> : detail::static_array_type_name<
	meta_type<ElementType>, -1
>
{
	typedef typename meta_type<ElementType>::scope scope;
	typedef ElementType reflected_type[];
};

/** Meta-types for references
 */
template <class ReferredToType>
struct meta_type<ReferredToType&> : detail::static_reference_type_name<
	meta_type<ReferredToType>
>
{
	typedef typename meta_type<ReferredToType>::scope scope;
	typedef ReferredToType& reflected_type; 
};

/** Meta-types for const types
 */
template <class NonConstType>
struct meta_type<const NonConstType> : detail::static_const_type_name<
	meta_type<NonConstType>
>
{
	typedef typename meta_type<NonConstType>::scope scope;
	typedef const NonConstType reflected_type; 
};

/** Meta-types for volatile types
 */
template <class NonVolatileType>
struct meta_type<volatile NonVolatileType> : detail::static_volatile_type_name<
	meta_type<NonVolatileType>
>
{
	typedef typename meta_type<NonVolatileType>::scope scope;
	typedef volatile NonVolatileType reflected_type; 
};

/** Meta-types for const volatile types
 */
template <class NonCVType>
struct meta_type<const volatile NonCVType> : detail::static_cv_type_name<
	meta_type<NonCVType>
>
{
	typedef typename meta_type<NonCVType>::scope scope;
	typedef const volatile NonCVType reflected_type; 
};


} // namespace mirror
} // namespace boost

#endif //include guard

