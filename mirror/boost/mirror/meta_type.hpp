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

// true type/false type for trait templates 
#include <boost/type_traits/integral_constant.hpp>
// meta namespaces (includes boost/char_type_switch/string.hpp)
#include <boost/mirror/meta_namespace.hpp>
// for declarations of meta-types for groups of types
#include <boost/preprocessor.hpp>

namespace boost {
namespace mirror {

/** meta-type template definition
 */
template <class base_type> 
struct meta_type;


/** Macro for declaration of meta-types
 */
#define BOOST_MIRROR_REG_META_TYPE(META_NAMESPACE_ALIAS, NAMESPACE, BASE_NAME)     \
	template <> struct meta_type< NAMESPACE::BASE_NAME >              \
	{                                                                 \
		typedef namespaces::META_NAMESPACE_ALIAS scope;                        \
		typedef NAMESPACE::BASE_NAME base_type;                                  \
		static const bchar*   base_name  (void) {return BOOST_STR_LIT(#BASE_NAME);}\
	};

/** Declaration of meta types for types in the global scope
 */
#define BOOST_MIRROR_REG_META_TYPE_GLOBAL_SCOPE(BASE_TYPE)   \
	template <> struct meta_type< BASE_TYPE >              \
	{                                                                 \
		typedef namespaces::_ scope;                        \
		typedef BASE_TYPE base_type;                                  \
		static const bchar*   base_name  (void) {return BOOST_STR_LIT(#BASE_TYPE);}\
	};

/** Declaration of meta types for types in declared inside
 *  of a class.
 */
#define BOOST_MIRROR_REG_META_TYPE_EMBEDDED(WRAPPER, BASE_TYPE)   \
	template <> struct meta_type< WRAPPER::BASE_TYPE >              \
	{                                                                 \
		typedef meta_type< WRAPPER > scope;                        \
		typedef WRAPPER::BASE_TYPE base_type;                                  \
		static const bchar*   base_name  (void) {return BOOST_STR_LIT(#BASE_TYPE);}\
	};


/** Helper macro used for batch registering of the meta-types for
 *  the C++ native types
 */
#define BOOST_MIRROR_REG_ITH_META_TYPE_NATIVE(I, _, BASE_TYPE)\
	BOOST_MIRROR_REG_META_TYPE_GLOBAL_SCOPE(BASE_TYPE)

#define BOOST_MIRROR_NATIVE_TYPES \
	BOOST_PP_TUPLE_TO_LIST( \
		14, \
		( \
			bool, \
			char, signed char, unsigned char, wchar_t, \
			unsigned short, short, \
			int, unsigned, \
			long, unsigned long, \
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


/** Meta-types for pointers
 */
template <class pointee_type>
struct meta_type<pointee_type*>
{
	typedef typename meta_type<pointee_type>::scope scope;
	typedef pointee_type* base_type; 
};

/** Meta-types for references
 */
template <class refered_to_type>
struct meta_type<refered_to_type&>
{
	typedef typename meta_type<refered_to_type>::scope scope;
	typedef refered_to_type& base_type; 
};

/** Meta-types for const types
 */
template <class non_const_type>
struct meta_type<const non_const_type>
{
	typedef typename meta_type<non_const_type>::scope scope;
	typedef const non_const_type base_type; 
};

/** Meta-types for volatile types
 */
template <class non_volatile_type>
struct meta_type<volatile non_volatile_type>
{
	typedef typename meta_type<non_volatile_type>::scope scope;
	typedef volatile non_volatile_type base_type; 
};

/** Meta-types for const volatile types
 */
template <class non_cv_type>
struct meta_type<const volatile non_cv_type>
{
	typedef typename meta_type<non_cv_type>::scope scope;
	typedef const volatile non_cv_type base_type; 
};


} // namespace mirror
} // namespace boost

#endif //include guard

