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

namespace boost {
namespace mirror {

/** meta-namespace template definition
 */
template<class namespace_alias> 
struct meta_namespace;

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
};

/** Helper macro for registering new general namespaces (top level or nested)
 */
#define BOOST_MIRROR_REG_META_NAMESPACE_HELPER(PARENT_NS_ALIAS, PREFIX, NAMESPACE_NAME) \
namespace namespaces {struct PREFIX##_##NAMESPACE_NAME { }; }\
template<> struct meta_namespace< namespaces :: PREFIX##_##NAMESPACE_NAME >      \
{                                                                                         \
	typedef meta_namespace< namespaces :: PARENT_NS_ALIAS > parent;                                                        \
	typedef mpl::push_back<typedef parent::scope, parent>::type scope;    \
	static const bchar*   base_name  (void) {return BOOST_STR_LIT(#NAMESPACE_NAME);}           \
};                                                                                        \


/** Macro for registering new general namespaces (top level or nested)
 */
#define BOOST_MIRROR_REG_META_NAMESPACE(PARENT_NS_ALIAS, NAMESPACE_NAME) \
	BOOST_MIRROR_REG_META_NAMESPACE_HELPER(PARENT_NS_ALIAS, PARENT_NS_ALIAS, NAMESPACE_NAME)

/** Macro for registering of top-level namespaces
 */
#define BOOST_MIRROR_REG_META_NAMESPACE_TOP_LEVEL(NAMESPACE_NAME)    \
	BOOST_MIRROR_REG_META_NAMESPACE_HELPER(\
		_, \
		BOOST_PP_EMPTY(), \
		NAMESPACE_NAME \
	) 

// Registration of the ::std namespace 
BOOST_MIRROR_REG_META_NAMESPACE_TOP_LEVEL(std)
// Registration of the ::boost namespace 
BOOST_MIRROR_REG_META_NAMESPACE_TOP_LEVEL(boost)
// Registration of the ::boost::mirror namespace 
BOOST_MIRROR_REG_META_NAMESPACE(_boost, mirror)

/** Macro that expands into the meta_namespace for the 
 *  namespace with the given alias.
 */
#define BOOST_MIRROR_REFLECT_NAMESPACE(NAMESPACE_ALIAS) \
	::boost::mirror::meta_namespace<namespaces::NAMESPACE_ALIAS>

} // namespace mirror
} // namespace boost

#endif //include guard

