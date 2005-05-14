// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROPERTY_OBJECT_PROPERTY_HPP
#define BOOST_PROPERTY_OBJECT_PROPERTY_HPP

#include <boost/property/property_type.hpp>

namespace boost
{

/** Implement a property using member functions of an object. */

template
<
   typename T, class Object,
   T    ( Object::*getter )() const,
   void ( Object::*setter )( const T & )
>
class object_property: public property_type< T, Object * >
{
   Object * object;
   public:
      T get() const /**< get the value of the property. */
      {
         return ( object ->* getter )();
      }
      void set( const T & v ) /**< set the value of the property. */
      {
         ( object ->* setter )( v );
      }
   public:
      inline object_property( Object * o ):
         object( o )
      {
      }
};

}

#endif
