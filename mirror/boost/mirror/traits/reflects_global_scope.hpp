/**
 * \file boost/mirror/reflects_global_scope.hpp
 * Meta function that returns true if the given meta_namespace 
 * reflects the global scope
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_REFLECTS_GLOBAL_SCOPE_HPP
#define BOOST_MIRROR_META_REFLECTS_GLOBAL_SCOPE_HPP

// true type/false type for trait templates 
#include <boost/type_traits/integral_constant.hpp>
#include <boost/mirror/meta_namespace.hpp>

namespace boost {
namespace mirror {

/** Reflects-global-scope trait template for other meta-namespaces
 */
template <class MetaNamespace>
struct reflects_global_scope
: public false_type{ };

/** Reflects-global-scope trait specialization for global scope meta_namespace
 */
template <>
struct reflects_global_scope< BOOST_MIRRORED_GLOBAL_SCOPE() >
: public true_type{ };


} // namespace mirror
} // namespace boost

#endif //include guard

