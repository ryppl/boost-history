// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROPERTY_RANK1_OBJECT_PROPERTY_HPP
#define BOOST_PROPERTY_RANK1_OBJECT_PROPERTY_HPP

#include <boost/property/property_type.hpp>

namespace boost
{

/** Implement a rank 1 property using member functions of an object. */

template
<
   typename T, typename I, class Object,
   T    ( Object::*getter )( I ) const,
   void ( Object::*setter )( I, const T & )
>
class rank1_object_property: public property_type< T, Object *, T &, I >
{
   Object * object;
   public:
      T get( I i ) const /**< get the value of the property. */
      {
         return ( object ->* getter )( i );
      }
      void set( I i, const T & v ) /**< set the value of the property. */
      {
         ( object ->* setter )( i, v );
      }
   public:
      inline rank1_object_property( Object * o ):
         object( o )
      {
      }
};

}

#endif
