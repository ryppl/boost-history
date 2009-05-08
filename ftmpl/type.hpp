#ifndef BOOST_FTMPL_TYPE_HPP
#define BOOST_FTMPL_TYPE_HPP

namespace boost { namespace ftmpl {

template< typename T >
struct type_t;

template< typename T >
type_t< T >& type();

template< typename T >
type_t< T >& type( T& );

template< typename T >
type_t< T >& type( type_t< T >& );

} }


#endif // BOOST_FTMPL_TYPE_HPP
