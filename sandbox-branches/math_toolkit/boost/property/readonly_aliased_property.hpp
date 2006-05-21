// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROPERTY_READONLY_ALIASED_PROPERTY_HPP
#define BOOST_PROPERTY_READONLY_ALIASED_PROPERTY_HPP

#include <boost/property/property_type.hpp>

namespace boost
{

/** Implement a property that is an alias to a variable. */

template< typename T >
class readonly_aliased_property: public property_type< T, T & >
{
   T * value;
   public:
      inline T get() const; /**< get the value of the property. */
   public:
      inline readonly_aliased_property( T & v );
};

template< typename T >
T readonly_aliased_property< T >::get() const
{
   return *value;
}

template< typename T >
readonly_aliased_property< T >::readonly_aliased_property( T & v ): value( &v )
{
}

}

#endif
