// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROPERTY_VALUE_PROPERTY_HPP
#define BOOST_PROPERTY_VALUE_PROPERTY_HPP

#include <boost/property/property_type.hpp>

namespace boost
{

/** Implement a property where the value is stored in the property itself. */

template< typename T >
class value_property: public property_type< T >
{
   T value;
   public:
      inline T    get() const;        /**< get the value of the property. */
      inline void set( const T & v ); /**< set the value of the property. */
   public:
      inline value_property( T v );
};

template< typename T >
T value_property< T >::get() const
{
   return value;
}

template< typename T >
void value_property< T >::set( const T & v )
{
   value = v;
}

template< typename T >
value_property< T >::value_property( T v ): value( v )
{
}

}

#endif
