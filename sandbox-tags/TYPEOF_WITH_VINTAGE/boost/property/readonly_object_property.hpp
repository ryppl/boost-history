// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROPERTY_READONLY_OBJECT_PROPERTY_HPP
#define BOOST_PROPERTY_READONLY_OBJECT_PROPERTY_HPP

#include <boost/property/property_type.hpp>

namespace boost
{

/** Implement a readonly property using member functions of an object. */

template
<
   typename T, class Object,
   T ( Object::*getter )() const
>
class readonly_object_property: public property_type< T, Object * >
{
   Object * object;
   public:
      T get() const /**< get the value of the property. */
      {
         return ( object ->* getter )();
      }
   public:
      inline readonly_object_property( Object * o ):
         object( o )
      {
      }
};

}

#endif
