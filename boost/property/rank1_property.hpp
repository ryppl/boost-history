// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROPERTY_RANK1_PROPERTY_HPP
#define BOOST_PROPERTY_RANK1_PROPERTY_HPP

#include <boost/property/scalar_property.hpp>

namespace boost
{

/** Represents a rank 1 (1-dimensional) property.
 * Implements operator[] that provides access to a scalar property.
 */

template< typename Prop >
class rank1_property: public Prop
{
   inline rank1_property< Prop > & operator=( const rank1_property< Prop > & );
   inline rank1_property< Prop >( const rank1_property< Prop > & );
   public:
      typedef rank1_property< Prop >          this_type;
      typedef Prop                            property_type;
      typedef typename Prop::value_type       value_type;
      typedef typename Prop::index_type       index_type;
      typedef typename Prop::constructor_type constructor_type;
   public:
      /** Implement a scalar property for [] usage. */

      class scalar: public inherited_property_type< Prop, scalar >
      {
         friend class scalar_type;
         private:
            rank1_property< Prop > & r1;
            index_type               idx;
         public:
            /** get the value of the idx-th element. */
            value_type get() const
            {
               return r1.get( idx );
            }
            /** set the value of the idx-th element. */
            const value_type & set( const value_type & value )
            {
               return r1.set( idx, value );
            }
         public:
            scalar( rank1_property< Prop > & r, index_type i ):
               r1( r ), idx( i )
            {
            }
      };

      /** workaround for the lack of a public constructor in scalar_property. */

      class scalar_type: public scalar_property< scalar >
      {
         public:
            value_type operator=( const value_type & value )
            {
               return scalar_property< scalar >::operator=( value );
            }
         public:
            scalar_type( const scalar_type & s ):
               scalar_property< scalar >( scalar( s.r1, s.idx ))
            {
            }
            scalar_type( rank1_property< Prop > & r, index_type i ):
               scalar_property< scalar >( scalar( r, i ))
            {
            }
      };
   public:
      inline value_type get( index_type i ) const; /**< getter */
      inline value_type operator()( index_type i ) const; /**< getter */
   public:
      inline const value_type & set( index_type i, const value_type & v ); /**< setter */
      inline const value_type & operator()( index_type i, const value_type & v ); /**< setter */
   public:
      inline const scalar_type operator[]( index_type i ) const; /**< indexing */
      inline       scalar_type operator[]( index_type i ); /**< indexing */
   public:
      inline rank1_property( constructor_type );
};

// getters

template< typename Prop >
typename rank1_property< Prop >::value_type
rank1_property< Prop >::get( index_type i ) const
{
   return Prop::get( i );
}

template< typename Prop >
typename rank1_property< Prop >::value_type
rank1_property< Prop >::operator()( index_type i ) const
{
   return get( i );
}

// setters

template< typename Prop >
const typename rank1_property< Prop >::value_type &
rank1_property< Prop >::set( index_type i, const value_type & v )
{
   Prop::set( i, v );
   return v;
}

template< typename Prop >
const typename rank1_property< Prop >::value_type &
rank1_property< Prop >::operator()( index_type i, const value_type & v )
{
   return set( i, v );
}

// indexing

template< typename Prop >
const typename rank1_property< Prop >::scalar_type
rank1_property< Prop >::operator[]( index_type i ) const
{
   return scalar_type( *this, i );
}

template< typename Prop >
typename rank1_property< Prop >::scalar_type
rank1_property< Prop >::operator[]( index_type i )
{
   return scalar_type( *this, i );
}

// constructors

template< typename Prop >
rank1_property< Prop >::rank1_property( constructor_type v ):
   Prop( v )
{
}

}

#endif
