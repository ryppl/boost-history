#ifndef BOOST_FTMPL_VALUE_HPP
#define BOOST_FTMPL_VALUE_HPP

#include "type.hpp"

namespace boost { namespace ftmpl {

template< typename T, T Value >
struct value_t;

template< typename T, T Value >
value_t< T, Value >& value();

#ifdef BOOST_HAS_TEMPLATE_ALIASES

#define BOOST_FTMPL_VALUE_ALIAS( name, type )                                  \
template< type Value >                                                         \
using name ## _ ## t = value_t< type, Value >;

#else

#define BOOST_FTMPL_VALUE_ALIAS( name, type ) /**/

#endif

#define BOOST_FTMPL_VALUE_DEF( name, type )                                    \
BOOST_FTMPL_VALUE_ALIAS( name, type )                                    \
template< type Value >                                                         \
value_t< type, Value >& name();

BOOST_FTMPL_VALUE_DEF( bool_, bool )
BOOST_FTMPL_VALUE_DEF( char_, char )
BOOST_FTMPL_VALUE_DEF( short_, short )
BOOST_FTMPL_VALUE_DEF( int_, int )
BOOST_FTMPL_VALUE_DEF( long_, long )
BOOST_FTMPL_VALUE_DEF( wchar_t_, wchar_t )
BOOST_FTMPL_VALUE_DEF( schar, signed char )
BOOST_FTMPL_VALUE_DEF( uchar, unsigned char )
BOOST_FTMPL_VALUE_DEF( ushort, unsigned short )
BOOST_FTMPL_VALUE_DEF( uint, unsigned int )
BOOST_FTMPL_VALUE_DEF( ulong, unsigned long )
BOOST_FTMPL_VALUE_DEF( longlong, long long )
BOOST_FTMPL_VALUE_DEF( ulonglong, unsigned long long )

// ToDo: Account for integral promotion
#define MPL_UNARY_OP( op )                                                     \
template< typename Type, Type Value >                                          \
type_t< value_t< Type, ( op Value ) > >&                                 \
operator op( type_t< value_t< Type, Value > >& );

MPL_UNARY_OP( + )
MPL_UNARY_OP( - )
MPL_UNARY_OP( ! )
MPL_UNARY_OP( ~ )

// ToDo: Account for integral promotion
// Hack: Don't use LType
#define MPL_BINARY_OP( op )                                                    \
template< typename LType, LType LValue, typename RType, RType RValue >         \
type_t< value_t< LType, ( LValue op RValue ) > >&                              \
operator op( type_t< value_t< LType, LValue > >&, type_t< value_t< RType, RValue > >& );

MPL_BINARY_OP( + )
MPL_BINARY_OP( - )
MPL_BINARY_OP( * )
MPL_BINARY_OP( / )
MPL_BINARY_OP( ^ )
MPL_BINARY_OP( | )
MPL_BINARY_OP( & )
MPL_BINARY_OP( << )
MPL_BINARY_OP( >> )

#define MPL_BOOL_BINARY_OP( op )                                                    \
template< typename LType, LType LValue, typename RType, RType RValue >         \
type_t< value_t< bool, ( LValue op RValue ) > >&                 \
operator op( type_t< value_t< LType, LValue > >&, type_t< value_t< RType, RValue > >& );

MPL_BOOL_BINARY_OP( || )
MPL_BOOL_BINARY_OP( && )
MPL_BOOL_BINARY_OP( < )
MPL_BOOL_BINARY_OP( > )
MPL_BOOL_BINARY_OP( <= )
MPL_BOOL_BINARY_OP( >= )
MPL_BOOL_BINARY_OP( == )
MPL_BOOL_BINARY_OP( != )

typedef value_t< bool, true > true_t;
typedef value_t< bool, false > false_t;

type_t< true_t >& true_();
type_t< false_t >& false_();

template< typename T, T V >
type_t< T > value_type( type_t< value_t< T, V > >& );

template< typename T >
struct unwrap_value;

template< typename T, T V >
struct unwrap_value< type_t< value_t< T, V > > >
{
  static T const value = V;
};

//#define BOOST_FTMPL_

} }

#endif // BOOST_FTMPL_VALUE_HPP
