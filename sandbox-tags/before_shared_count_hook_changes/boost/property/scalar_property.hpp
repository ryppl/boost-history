// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROPERTY_SCALAR_PROPERTY_HPP
#define BOOST_PROPERTY_SCALAR_PROPERTY_HPP

#include <boost/property/property_type.hpp>
#include <boost/config.hpp>

#if defined(BOOST_MSVC)
#  pragma warning( push )
#  pragma warning( disable : 4181 ) // qualifier applied to reference type ignored
#endif

namespace boost
{

namespace detail
{

template< typename T >
class arrow_from_type
{
   T value;
   public:
      inline T * operator->();
      inline const T * operator->() const;
      inline arrow_from_type( const T & v );
};

template< typename T >
T * arrow_from_type< T >::operator->()
{
   return &value;
}

template< typename T >
const T * arrow_from_type< T >::operator->() const
{
   return &value;
}

template< typename T >
arrow_from_type< T >::arrow_from_type( const T & v ): value( v )
{
}

}

/** Represents a scalar value property.
 * Implements a complete set of C++ operators for intuitive usage.
 */

template< typename Prop >
class scalar_property: public Prop
{
   inline scalar_property< Prop > & operator=( const scalar_property< Prop > & );
   inline scalar_property( const scalar_property< Prop > & );
   public:
      typedef scalar_property< Prop >         this_type;
      typedef Prop                            property_type;
      typedef typename Prop::value_type       value_type;
      typedef typename Prop::constructor_type constructor_type;
      typedef typename Prop::index_type       index_type;
      typedef typename Prop::subscript_type   subscript_type;
      typedef detail::arrow_from_type< value_type >
                                              arrow_type;
   public:
      inline value_type get() const; /**< getter */
      inline value_type operator()() const; /**< getter */
      inline operator value_type() const; /**< getter */
      inline arrow_type operator->(); /**< getter */
      inline const arrow_type operator->() const; /**< getter */
   public:
      inline const value_type & set( const value_type & v ); /**< setter */
      inline const value_type & operator()( const value_type & v ); /**< setter */
      inline const value_type & operator=(  const value_type & v ); /**< setter */
   public: // C++ native interface
      inline const this_type & operator+=(  const value_type & v );
      inline const this_type & operator-=(  const value_type & v );
      inline const this_type & operator*=(  const value_type & v );
      inline const this_type & operator/=(  const value_type & v );
      inline const this_type & operator%=(  const value_type & v );
      inline const this_type & operator<<=( const value_type & v );
      inline const this_type & operator>>=( const value_type & v );
      inline const this_type & operator&=(  const value_type & v );
      inline const this_type & operator|=(  const value_type & v );
      inline const this_type & operator^=(  const value_type & v );

      inline const this_type & operator++();
      inline value_type operator++( int );
      inline const this_type & operator--();
      inline value_type operator--( int );

      inline subscript_type operator[]( index_type i );
      inline const subscript_type operator[]( index_type i ) const;
   public:
      inline scalar_property( constructor_type );
};

// getters

template< typename Prop >
typename scalar_property< Prop >::value_type
scalar_property< Prop >::get() const
{
   return Prop::get();
}

template< typename Prop >
typename scalar_property< Prop >::value_type
scalar_property< Prop >::operator()() const
{
   return get();
}

template< typename Prop >
scalar_property< Prop >::operator
typename scalar_property< Prop >::value_type() const
{
   return get();
}

template< typename Prop >
typename scalar_property< Prop >::arrow_type
scalar_property< Prop >::operator->()
{
   return arrow_type( get());
}

template< typename Prop >
const typename scalar_property< Prop >::arrow_type
scalar_property< Prop >::operator->() const
{
   return arrow_type( get());
}

// setters

template< typename Prop >
const typename scalar_property< Prop >::value_type &
scalar_property< Prop >::set( const value_type & value )
{
   Prop::set( value );
   return value;
}

template< typename Prop >
const typename scalar_property< Prop >::value_type &
scalar_property< Prop >::operator()( const value_type & value )
{
   return set( value );
}

template< typename Prop >
const typename scalar_property< Prop >::value_type &
scalar_property< Prop >::operator=( const value_type & value )
{
   return set( value );
}

// C++ native interface

template< typename Prop >
const typename scalar_property< Prop >::this_type &
scalar_property< Prop >::operator+=( const value_type & value )
{
   set( get() + value );
   return *this;
}

template< typename Prop >
const typename scalar_property< Prop >::this_type &
scalar_property< Prop >::operator-=( const value_type & value )
{
   set( get() - value );
   return *this;
}

template< typename Prop >
const typename scalar_property< Prop >::this_type &
scalar_property< Prop >::operator*=( const value_type & value )
{
   set( get() * value );
   return *this;
}

template< typename Prop >
const typename scalar_property< Prop >::this_type &
scalar_property< Prop >::operator/=( const value_type & value )
{
   set( get() / value );
   return *this;
}

template< typename Prop >
const typename scalar_property< Prop >::this_type &
scalar_property< Prop >::operator%=( const value_type & value )
{
   set( get() % value );
   return *this;
}

template< typename Prop >
const typename scalar_property< Prop >::this_type &
scalar_property< Prop >::operator<<=( const value_type & value )
{
   set( get() << value );
   return *this;
}

template< typename Prop >
const typename scalar_property< Prop >::this_type &
scalar_property< Prop >::operator>>=( const value_type & value )
{
   set( get() >> value );
   return *this;
}

template< typename Prop >
const typename scalar_property< Prop >::this_type &
scalar_property< Prop >::operator&=( const value_type & value )
{
   set( get() & value );
   return *this;
}

template< typename Prop >
const typename scalar_property< Prop >::this_type &
scalar_property< Prop >::operator|=( const value_type & value )
{
   set( get() | value );
   return *this;
}

template< typename Prop >
const typename scalar_property< Prop >::this_type &
scalar_property< Prop >::operator^=( const value_type & value )
{
   set( get() ^ value );
   return *this;
}

template< typename Prop >
const typename scalar_property< Prop >::this_type &
scalar_property< Prop >::operator++()
{
   return *this += 1;
}

template< typename Prop >
typename scalar_property< Prop >::value_type
scalar_property< Prop >::operator++( int )
{
   value_type value = get();
   ++*this;
   return value;
}

template< typename Prop >
const typename scalar_property< Prop >::this_type &
scalar_property< Prop >::operator--()
{
   return *this -= 1;
}

template< typename Prop >
typename scalar_property< Prop >::value_type
scalar_property< Prop >::operator--( int )
{
   value_type value = get();
   --*this;
   return value;
}

template< typename Prop >
typename scalar_property< Prop >::subscript_type
scalar_property< Prop >::operator[]( index_type i )
{
   return get()[ i ];
}

template< typename Prop >
const typename scalar_property< Prop >::subscript_type
scalar_property< Prop >::operator[]( index_type i ) const
{
   return get()[ i ];
}

// constructors

template< typename Prop >
scalar_property< Prop >::scalar_property( constructor_type v ):
   Prop( v )
{
}

}

#if defined(BOOST_MSVC)
#  pragma warning( pop )
#endif

#endif
