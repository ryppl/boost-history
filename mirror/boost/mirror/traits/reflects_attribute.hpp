/**
 * \file boost/mirror/reflects_attribute.hpp
 * Meta function that returns true if the given template argument
 * is an instantiation of meta_attribute
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_REFLECTS_ATTRIBUTE_HPP
#define BOOST_MIRROR_META_REFLECTS_ATTRIBUTE_HPP

// true type/false type for trait templates 
#include <boost/type_traits/integral_constant.hpp>
#include <boost/mirror/meta_attributes.hpp>

namespace boost {
namespace mirror {

template <class MetaObject>
struct reflects_inheritance : public false_type { };

template <
    class ReflectedType,
    class VariantTag,
    class MetaAttributes,
    class AttribPos
>
struct reflects_attribute<
    meta_class_attribute<
        ReflectedType,
        VariantTag,
        MetaAttributes,
        AttribPos
    > 
> : public true_type{ };


} // namespace mirror
} // namespace boost

#endif //include guard

