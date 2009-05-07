#ifndef BOOST_FTMPL_VECTOR_HPP
#define BOOST_FTMPL_VECTOR_HPP

#include "size_t.hpp"
#include "type.hpp"

namespace boost { namespace ftmpl {

template< typename ... T >
struct vector_t;

template< typename ... T >
type_t< vector_t< T... > >& vector();

template< typename ... T >
type_t< vector_t< T... > >& vector( T& ... );

template< typename ... T, typename Back >
type_t< vector_t< T..., Back > >& push_back( type_t< vector_t< T... > >&, Back& );

template< typename ... T, typename Last >
type_t< vector_t< T... > >& pop_back( type_t< vector_t< T..., Last > >& );

template< typename First, typename ... Tail >
type_t< vector_t< Tail... > >& pop_front( type_t< vector_t< First, Tail... > >& );

template< typename ... T, typename Tail >
Tail& back( type_t< vector_t< T..., Tail > >& );

template< typename Head, typename ... T >
Head& front( type_t< vector_t< Head, T... > >& );

template< typename ... T >
type_t< value_t< ::std::size_t, sizeof...( T ) > >& size( type_t< vector_t< T... > >& );

} }

#endif
