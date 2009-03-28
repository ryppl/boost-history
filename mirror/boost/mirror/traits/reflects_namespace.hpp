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
#include <boost/mpl/bool.hpp>
#include <boost/mirror/meta_namespace.hpp>

namespace boost {
namespace mirror {

template <class MetaObject>
struct reflects_namespace : public mpl::false_{ };

template <class NamespaceAlias>
struct reflects_namespace<meta_namespace<NamespaceAlias> > : public mpl::true_{ };


} // namespace mirror
} // namespace boost

#endif //include guard

