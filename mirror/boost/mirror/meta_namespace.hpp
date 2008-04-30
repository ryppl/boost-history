/**
 * \file boost/mirror/meta_namespace.hpp
 * Registering and reflection of namespaces
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_NAMESPACE_HPP
#define BOOST_MIRROR_META_NAMESPACE_HPP

// for various typelists
#include <boost/mpl/vector.hpp>
// common mpl algoritgms
#include <boost/mpl/push_back.hpp>
//
// for bulk declarations
#include <boost/preprocessor.hpp>

// mirror uses switchable character type
#include <boost/char_type_switch/string.hpp>
// forward declarations
#include <boost/mirror/meta_data_fwd.hpp>
// implementation helpers
#include <boost/mirror/common_defs.hpp>

namespace boost {
namespace mirror {

/** defines an alias (_) for the meta-namespace for the global scope
 */
namespace namespaces { struct _{ }; }

/** meta-namespace for the global scope 
 */
template<> struct meta_namespace< namespaces::_ >
{
	typedef void parent;
	typedef mpl::vector0<> scope;

	static const bchar* base_name(void) {return BOOST_STR_LIT("");}
	BOOST_STATIC_CONSTANT(int, base_name_length = 0);

	static const bchar* full_name(void) {return BOOST_STR_LIT("");}
	BOOST_STATIC_CONSTANT(int, full_name_length = 0);
};


/** Helper macro for registering new general namespaces (top level or nested)
 */
#define BOOST_MIRROR_REG_META_NAMESPACE_HELPER(PARENT_NS_ALIAS, NAMESPACE_NAME) \
{                                                                                         \
	typedef meta_namespace< namespaces :: PARENT_NS_ALIAS > parent;                                                        \
	typedef mpl::push_back<parent::scope, parent>::type scope;    \
	BOOST_STATIC_CONSTANT(\
		int, \
		base_name_length = \
		BOOST_STR_LIT_LENGTH(#NAMESPACE_NAME)\
	); \
	static const bchar* base_name(void) {return BOOST_STR_LIT(#NAMESPACE_NAME);}           \
	BOOST_STATIC_CONSTANT(\
		int, \
		full_name_length = \
		parent::full_name_length + 2 + \
		base_name_length \
	); \
	static const bchar* full_name(void)\
	{\
		static bchar the_full_name[full_name_length] = \
			BOOST_STR_LIT(""); \
		if(!the_full_name[0])  \
		{ \
			bchar * pos = the_full_name; \
			bstrncpy(pos, parent::full_name(), parent::full_name_length);\
			pos += parent::full_name_length; \
			bstrncpy(pos, BOOST_STR_LIT("::"), 2);\
			pos += 2; \
			bstrncpy(pos, base_name(), base_name_length);\
		} \
		return the_full_name; \
	}           \
};                                                                                        


/** Macro for registering new general namespaces (top level or nested)
 */
#define BOOST_MIRROR_REG_META_NAMESPACE(PARENT_NS_ALIAS, NAMESPACE_NAME) \
	namespace namespaces {struct PARENT_NS_ALIAS##_##NAMESPACE_NAME { }; }\
	template<> struct meta_namespace< namespaces :: PARENT_NS_ALIAS##_##NAMESPACE_NAME >      \
	BOOST_MIRROR_REG_META_NAMESPACE_HELPER(PARENT_NS_ALIAS, NAMESPACE_NAME)

/** Macro for registering of top-level namespaces
 */
#define BOOST_MIRROR_REG_META_NAMESPACE_TOP_LEVEL(NAMESPACE_NAME)    \
	namespace namespaces {struct _##NAMESPACE_NAME { }; }\
	template<> struct meta_namespace< namespaces :: _##NAMESPACE_NAME >      \
	BOOST_MIRROR_REG_META_NAMESPACE_HELPER(_, NAMESPACE_NAME)

// Registration of the ::std namespace 
BOOST_MIRROR_REG_META_NAMESPACE_TOP_LEVEL(std)
// Registration of the ::boost namespace 
BOOST_MIRROR_REG_META_NAMESPACE_TOP_LEVEL(boost)
// Registration of the ::boost::mirror namespace 
BOOST_MIRROR_REG_META_NAMESPACE(_boost, mirror)


} // namespace mirror
} // namespace boost

#endif //include guard

