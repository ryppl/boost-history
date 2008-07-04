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

#include <boost/preprocessor/list/for_each.hpp>

// meta namespaces
#include <boost/mirror/meta_namespace.hpp>
//
// type name decorations
#include <boost/mirror/detail/decorated_type_name.hpp>

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
	inline static const bstring& base_name(void)
	{
		return get_name(mpl::false_());
	}

	inline static const bstring& full_name(void)
	{
		return get_name(mpl::true_());
	}

	typedef detail::registered_type_info<Type> base_info;
	typedef typename base_info::scope scope;
	typedef typename base_info::reflected_type reflected_type;
};

/** Helper macro used to declare base-name getting functions
 *  and base-name length static constants
 */
#define BOOST_MIRROR_REG_TYPE_DECLARE_BASE_NAME_HELPER(TYPE_NAME) \
	static const bstring& get_name(mpl::false_) \
	{ \
		static bstring s_name(BOOST_STR_LIT(#TYPE_NAME)); \
		return s_name; \
	} 

/** Macro for registering global-scope types
 */
#define BOOST_MIRROR_REG_GLOBAL_SCOPE_TYPE(BASE_NAME) \
	namespace detail { \
	template <> struct registered_type_info< BASE_NAME > \
	{ \
		typedef BOOST_MIRRORED_GLOBAL_SCOPE() scope; \
		typedef BASE_NAME reflected_type; \
		BOOST_MIRROR_REG_TYPE_DECLARE_BASE_NAME_HELPER(BASE_NAME) \
	}; \
	} // namespace detail

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

/** Macro that expands into a typedef-ined type selectos
 */
#define BOOST_MIRROR_GET_TYPEDEFD_TYPE_SELECTOR(NAMESPACE, TYPEDEFD_NAME) \
	::boost::mirror::typedef_::TYPEDEFD_NAME < \
		BOOST_MIRRORED_NAMESPACE( NAMESPACE ) \
	> \

/** Macro for registering typedef-ined types in namespaces
 */
#define BOOST_MIRROR_REG_TYPEDEF(NAMESPACE, TYPEDEFD_NAME) \
	namespace typedef_ { \
		template <class MetaNamespace> struct TYPEDEFD_NAME; \
		template <> struct TYPEDEFD_NAME< \
			BOOST_MIRRORED_NAMESPACE( NAMESPACE ) \
		> { }; \
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
	template <> struct meta_type< WRAPPER::BASE_NAME > \
	{ \
		typedef meta_type< WRAPPER > scope; \
		typedef WRAPPER::BASE_NAME reflected_type; \
		BOOST_MIRROR_REG_TYPE_DECLARE_BASE_NAME_HELPER(BASE_NAME) \
	};

/** Register C++ native types
 */
BOOST_MIRROR_REG_GLOBAL_SCOPE_TYPE(void)
BOOST_MIRROR_REG_GLOBAL_SCOPE_TYPE(bool)
BOOST_MIRROR_REG_GLOBAL_SCOPE_TYPE(char)
BOOST_MIRROR_REG_GLOBAL_SCOPE_TYPE(unsigned char)
BOOST_MIRROR_REG_GLOBAL_SCOPE_TYPE(signed char)
BOOST_MIRROR_REG_GLOBAL_SCOPE_TYPE(wchar_t)
BOOST_MIRROR_REG_GLOBAL_SCOPE_TYPE(short int)
BOOST_MIRROR_REG_GLOBAL_SCOPE_TYPE(unsigned short int)
BOOST_MIRROR_REG_GLOBAL_SCOPE_TYPE(int)
BOOST_MIRROR_REG_GLOBAL_SCOPE_TYPE(unsigned int)
BOOST_MIRROR_REG_GLOBAL_SCOPE_TYPE(long int)
BOOST_MIRROR_REG_GLOBAL_SCOPE_TYPE(unsigned long int)
BOOST_MIRROR_REG_GLOBAL_SCOPE_TYPE(float)
BOOST_MIRROR_REG_GLOBAL_SCOPE_TYPE(double)

/** Register std string and wstring
 */
BOOST_MIRROR_REG_TYPE(::std, string)
BOOST_MIRROR_REG_TYPE(::std, wstring)
/** Now register the bchar and bstring too
 */
BOOST_MIRROR_REG_TYPEDEF(::boost, bchar)
BOOST_MIRROR_REG_TYPEDEF(::boost, bstring)

/** Meta-types for pointers
 */
template <class PointeeType>
struct meta_type<PointeeType*> : detail::decorated_type_name<
	meta_type<PointeeType>,
	detail::type_name_decorator<PointeeType*>
>
{
	typedef typename meta_type<PointeeType>::scope scope;
	typedef PointeeType* reflected_type; 
};



/** Meta-types for arrays
 */
template <class ElementType, size_t Size>
struct meta_type<ElementType[Size]> : detail::decorated_type_name<
	meta_type<ElementType>,
	detail::type_name_decorator<ElementType[Size]>
>
{
	typedef typename meta_type<ElementType>::scope scope;
	typedef ElementType reflected_type[Size];
};


template <class ElementType, size_t Size>
struct meta_type<const ElementType[Size]> : detail::decorated_type_name<
	meta_type<const ElementType>,
	detail::type_name_decorator<ElementType[Size]>
>
{
	typedef typename meta_type<ElementType>::scope scope;
	typedef const ElementType reflected_type[Size];
};

template <class ElementType, size_t Size>
struct meta_type<volatile ElementType[Size]> : detail::decorated_type_name<
	meta_type<volatile ElementType>,
	detail::type_name_decorator<ElementType[Size]>
>
{
	typedef typename meta_type<ElementType>::scope scope;
	typedef volatile ElementType reflected_type[Size];
};

template <class ElementType, size_t Size>
struct meta_type<const volatile ElementType[Size]>
: detail::decorated_type_name<
	meta_type<const volatile ElementType>,
	detail::type_name_decorator<ElementType[Size]>
>
{
	typedef typename meta_type<ElementType>::scope scope;
	typedef const volatile ElementType reflected_type[Size];
};

template <class ElementType>
struct meta_type<ElementType[]> : detail::decorated_type_name<
	meta_type<ElementType>,
	detail::type_name_decorator<ElementType[]>
>
{
	typedef typename meta_type<ElementType>::scope scope;
	typedef ElementType reflected_type[];
};

template <class ElementType>
struct meta_type<const ElementType []> : detail::decorated_type_name<
	meta_type<const ElementType>,
	detail::type_name_decorator<ElementType []>
>
{
	typedef typename meta_type<ElementType>::scope scope;
	typedef ElementType const reflected_type [];
};

template <class ElementType>
struct meta_type<volatile ElementType []> : detail::decorated_type_name<
	meta_type<volatile ElementType>,
	detail::type_name_decorator<ElementType []>
>
{
	typedef typename meta_type<ElementType>::scope scope;
	typedef ElementType volatile reflected_type [];
};

template <class ElementType>
struct meta_type<const volatile ElementType []> 
: detail::decorated_type_name<
	meta_type<const volatile ElementType>,
	detail::type_name_decorator<ElementType []>
>
{
	typedef typename meta_type<ElementType>::scope scope;
	typedef ElementType const volatile reflected_type [];
};



/** Meta-types for references
 */
template <class ReferredToType>
struct meta_type<ReferredToType&> : detail::decorated_type_name<
	meta_type<ReferredToType>,
	detail::type_name_decorator<ReferredToType&>
>
{
	typedef typename meta_type<ReferredToType>::scope scope;
	typedef ReferredToType& reflected_type; 
};

/** Meta-types for const types
 */
template <class NonConstType>
struct meta_type<const NonConstType> : detail::decorated_type_name<
	meta_type<NonConstType>,
	detail::type_name_decorator<const NonConstType>
>
{
	typedef typename meta_type<NonConstType>::scope scope;
	typedef const NonConstType reflected_type; 
};

/** Meta-types for volatile types
 */
template <class NonVolatileType>
struct meta_type<volatile NonVolatileType> : detail::decorated_type_name<
	meta_type<NonVolatileType>,
	detail::type_name_decorator<volatile NonVolatileType>
>
{
	typedef typename meta_type<NonVolatileType>::scope scope;
	typedef volatile NonVolatileType reflected_type; 
};

/** Meta-types for const volatile types
 */
template <class NonCVType>
struct meta_type<const volatile NonCVType> : detail::decorated_type_name<
	meta_type<NonCVType>,
	detail::type_name_decorator<const volatile NonCVType>
>
{
	typedef typename meta_type<NonCVType>::scope scope;
	typedef const volatile NonCVType reflected_type; 
};

} // namespace mirror
} // namespace boost

#endif //include guard

