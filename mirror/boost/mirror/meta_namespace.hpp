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
	static const bchar*   base_name  (void) {return BOOST_STR_LIT("");}
	BOOST_MIRROR_CONST_MEMBER_ATTRIB(size_t, base_name_length, 0)
};


/** Helper macro for registering new general namespaces (top level or nested)
 */
#define BOOST_MIRROR_REG_META_NAMESPACE_HELPER(PARENT_NS_ALIAS, NAMESPACE_NAME) \
{                                                                                         \
	typedef meta_namespace< namespaces :: PARENT_NS_ALIAS > parent;                                                        \
	typedef mpl::push_back<parent::scope, parent>::type scope;    \
	static const bchar*   base_name  (void) {return BOOST_STR_LIT(#NAMESPACE_NAME);}           \
	BOOST_MIRROR_CONST_MEMBER_ATTRIB(\
		size_t, \
		base_name_length, \
		BOOST_STR_LIT_LENGTH(#NAMESPACE_NAME)\
	) \
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

