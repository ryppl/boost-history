// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROPERTY_RANK1_READONLY_OBJECT_PROPERTY_HPP
#define BOOST_PROPERTY_RANK1_READONLY_OBJECT_PROPERTY_HPP

#include <boost/property/property_type.hpp>

namespace boost
{

/** Implement a readonly rank 1 property using member functions of an object. */

template
<
   typename T, typename I, class Object,
   T ( Object::*getter )( I ) const
>
class rank1_readonly_object_property: public property_type
                                      <
                                         T, Object *, T &, I
                                      >
{
   Object * object;
   public:
      T get( I ) const /**< get the value of the property. */
      {
         return ( object ->* getter )( i );
      }
   public:
      inline rank1_readonly_object_property( Object * o ):
         object( o )
      {
      }
};

}

#endif
