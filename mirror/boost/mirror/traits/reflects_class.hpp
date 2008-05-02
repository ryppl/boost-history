/**
 * \file boost/mirror/reflects_class.hpp
 * Meta function that returns true if the given meta-object
 * reflects a class
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_REFLECTS_CLASS_HPP
#define BOOST_MIRROR_META_REFLECTS_CLASS_HPP

// true type/false type for trait templates 
#include <boost/type_traits/integral_constant.hpp>
#include <boost/mirror/meta_class.hpp>

namespace boost {
namespace mirror {

template <class MetaObject>
struct reflects_class : public false_type{ };

template <class Class>
struct reflects_class<meta_class<Class> > : public true_type{ };


} // namespace mirror
} // namespace boost

#endif //include guard

