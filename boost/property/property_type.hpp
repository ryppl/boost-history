// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROPERTY_PROPERTY_TYPE_HPP
#define BOOST_PROPERTY_PROPERTY_TYPE_HPP

namespace boost
{

template< typename T, typename C = T, typename S = T &, typename I = int >
struct property_type
{
   typedef T value_type;
   typedef S subscript_type;
   typedef I index_type;
   typedef C constructor_type;
};

template< typename Prop, typename C = Prop >
struct inherited_property_type
{
   typedef typename Prop::value_type     value_type;
   typedef typename Prop::subscript_type subscript_type;
   typedef typename Prop::index_type     index_type;
   typedef C                             constructor_type;
};

}

#endif
