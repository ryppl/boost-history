/**
 * \file boost/mirror/reflects_types.hpp
 * Meta function that returns true if the given meta-object
 * reflects a type
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_REFLECTS_TYPE_HPP
#define BOOST_MIRROR_META_REFLECTS_TYPE_HPP

// true type/false type for trait templates 
#include <boost/type_traits/integral_constant.hpp>
#include <boost/mirror/meta_type.hpp>

namespace boost {
namespace mirror {

template <class MetaObject>
struct reflects_type : public false_type{ };

template <class Type>
struct reflects_type<meta_type<Type> > : public true_type{ };


template <class Class>
struct reflects_type<meta_class<Class> > : public true_type{ };


} // namespace mirror
} // namespace boost

#endif //include guard

