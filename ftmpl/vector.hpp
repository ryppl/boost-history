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

template< typename ... T, typename Front >
type_t< vector_t< Front, T...> >& push_front( type_t< vector_t< T... > >&, Front& );

template< typename First, typename ... Tail >
type_t< vector_t< Tail... > >& pop_front( type_t< vector_t< First, Tail... > >& );

template< typename ... T, typename Tail >
Tail& back( type_t< vector_t< T..., Tail > >& );

template< typename Head, typename ... T >
Head& front( type_t< vector_t< Head, T... > >& );

template< typename Head, typename ... T>
auto at( type_t< vector_t< Head, T... > >&, type_t< value_t< std::size_t, 0u > >& )
-> decltype( type< Head >() );

template< typename Head, typename ... T, std::size_t I>
auto at( type_t< vector_t< Head, T... > >&, type_t< value_t< std::size_t, I > >& )
-> decltype( at( vector<T ...>(), value<std::size_t, I - 1>() ) );

template< typename ... T >
type_t< value_t< ::std::size_t, sizeof...( T ) > >& size( type_t< vector_t< T... > >& );

} }

#endif
