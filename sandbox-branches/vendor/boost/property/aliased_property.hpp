// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROPERTY_ALIASED_PROPERTY_HPP
#define BOOST_PROPERTY_ALIASED_PROPERTY_HPP

#include <boost/property/property_type.hpp>

namespace boost
{

/** Implement a property that is an alias to a variable. */

template< typename T >
class aliased_property: public property_type< T, T & >
{
   T * value;
   public:
      inline T    get() const;        /**< get the value of the property. */
      inline void set( const T & v ); /**< set the value of the property. */
   public:
      inline aliased_property( T & v );
};

template< typename T >
T aliased_property< T >::get() const
{
   return *value;
}

template< typename T >
void aliased_property< T >::set( const T & v )
{
   *value = v;
}

template< typename T >
aliased_property< T >::aliased_property( T & v ): value( &v )
{
}

}

#endif
