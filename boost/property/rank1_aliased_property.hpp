// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROPERTY_RANK1_ALIASED_PROPERTY_HPP
#define BOOST_PROPERTY_RANK1_ALIASED_PROPERTY_HPP

#include <boost/property/property_type.hpp>

namespace boost
{

/** Implement a property that is aliased to a rank 1 array. */

template< typename T, typename S = T & >
class rank1_aliased_property: public property_type< T, T *, S >
{
   T * value;
   public:
      /** get the ith value of the property. */
      inline T    get( int i ) const;
      /** set the ith value of the property. */
      inline void set( int i, const T & v );
   public:
      inline rank1_aliased_property( T * v );
};

template< typename T, typename S >
T rank1_aliased_property< T, S >::get( int i ) const
{
   return value[ i ];
}

template< typename T, typename S >
void rank1_aliased_property< T, S >::set( int i, const T & v )
{
   value[ i ] = v;
}

template< typename T, typename S >
rank1_aliased_property< T, S >::rank1_aliased_property( T * v ): value( v )
{
}

}

#endif
