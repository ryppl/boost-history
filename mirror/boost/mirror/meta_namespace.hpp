/**
 * \file boost/mirror/meta_namespace.hpp
 * Registering and reflection of namespaces
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_NAMESPACE
#define BOOST_MIRROR_META_NAMESPACE

// true type/false type for trait templates 
#include <boost/type_traits/integral_constant.hpp>
// for various typelists
#include <boost/mpl/vector.hpp>
// for "compile time strings"
#include <boost/mpl/vector_c.hpp>
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
template<class parent_meta_ns, class namespace_name> 
struct meta_namespace;

/** meta-namespace for the global scope 
 */
template<> struct meta_namespace<void, mpl::vector_c<char> >
{
	typedef void parent;
	typedef mpl::vector0<> scope;
	static const bchar*   base_name  (void) {return BOOST_STR_LIT("");}
};
/** defines an alias (_) for the meta-namespace for the global scope
 */
namespace namespaces {
typedef meta_namespace<void, mpl::vector_c<char> > _;
}

/** Is-global-scope trait template for namespaces 
 */
template <class meta_namespace>
struct is_global_scope : public false_type{ };

/** Is-global-scope trait specialization for global scope meta_namespace
 */
template <>
struct is_global_scope<namespaces::_> : public true_type{ };


/** Helper macro for registering new general namespaces (top level or nested)
 */
#define BOOST_MIRROR_REG_META_NAMESPACE_HELPER(PARENT_META_NS, PREFIX, NAMESPACE, SPELLED_NAME) \
template<> struct meta_namespace<PARENT_META_NS, mpl::vector_c<char, SPELLED_NAME> >      \
{                                                                                         \
	typedef PARENT_META_NS parent;                                                        \
	typedef mpl::push_back<typedef PARENT_META_NS::scope, PARENT_META_NS>::type scope;    \
	static const bchar*   base_name  (void) {return BOOST_STR_LIT(#NAMESPACE);}           \
};                                                                                        \
namespace namespaces {                                                                            \
	typedef meta_namespace<PARENT_META_NS, mpl::vector_c<char, SPELLED_NAME> > PREFIX##_##NAMESPACE; \
}

/** Macro for registering new general namespaces (top level or nested)
 */
#define BOOST_MIRROR_REG_META_NAMESPACE(PARENT_META_NS_ALIAS, NAMESPACE, SPELLED_NAME) \
	BOOST_MIRROR_REG_META_NAMESPACE_HELPER(namespaces::PARENT_META_NS_ALIAS, PARENT_META_NS_ALIAS, NAMESPACE, SPELLED_NAME) 


/** Macro for registering of top-level namespaces
 */
#define BOOST_MIRROR_REG_META_NAMESPACE_TOP_LEVEL(NAMESPACE, SPELLED_NAME)    \
	BOOST_MIRROR_REG_META_NAMESPACE_HELPER(namespaces::_, BOOST_PP_EMPTY(), NAMESPACE, SPELLED_NAME) 

// Registration of the ::std namespace 
BOOST_MIRROR_REG_META_NAMESPACE_TOP_LEVEL(std, BOOST_PP_TUPLE_REM_CTOR(3,('s','t','d')));
// Registration of the ::boost namespace 
BOOST_MIRROR_REG_META_NAMESPACE_TOP_LEVEL(boost, BOOST_PP_TUPLE_REM_CTOR(5,('b','o','o','s','t')));
// Registration of the ::boost::mirror namespace 
BOOST_MIRROR_REG_META_NAMESPACE(_boost, mirror, BOOST_PP_TUPLE_REM_CTOR(6,('m','i','r','r','o','r')));

} // namespace mirror
} // namespace boost

#endif //include guard

