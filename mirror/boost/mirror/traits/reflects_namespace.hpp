/**
 * \file boost/mirror/reflects_namespace.hpp
 * Meta function that returns true if the given meta-object
 * reflects a namespace
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_REFLECTS_NAMESPACE_HPP
#define BOOST_MIRROR_META_REFLECTS_NAMESPACE_HPP

// true type/false type for trait templates 
#include <boost/type_traits/integral_constant.hpp>
#include <boost/mirror/meta_namespace.hpp>

namespace boost {
namespace mirror {

template <class meta_object>
struct reflects_namespace : public false_type{ };

template <class namespace_alias>
struct reflects_namespace<meta_namespace<namespace_alias> > : public true_type{ };


} // namespace mirror
} // namespace boost

#endif //include guard

