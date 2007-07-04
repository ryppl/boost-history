/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_TYPE_TRAITS_IS_INTERLOCKED_INTEGER_HPP
#define BOOST_ACT_INTERLOCKED_TYPE_TRAITS_IS_INTERLOCKED_INTEGER_HPP

#include <boost/act/config/interlocked/has_int.hpp>
#include <boost/mpl/contains.hpp>

#if BOOST_ACT_INTERLOCKED_HAS_INT_LEAST( 1 )

#include <boost/act/interlocked/integer/types.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/void.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/cat.hpp>

#define BOOST_ACT_INTERLOCKED_IS_INTERLOCKED_INTEGER_DETAIL_ELEM( size )       \
, BOOST_PP_CAT( BOOST_PP_CAT( int_least, size ), _t )                          \
, BOOST_PP_CAT( BOOST_PP_CAT( uint_least, size ), _t )

#define BOOST_ACT_INTERLOCKED_IS_INTERLOCKED_INTEGER_DETAIL_ELEM_DUMMY( size )

#define BOOST_ACT_INTERLOCKED_IS_INTERLOCKED_INTEGER_DETAIL_ELEM_IF( size )    \
BOOST_PP_IF                                                                    \
( BOOST_ACT_INTERLOCKED_HAS_INT_LEAST( size )                                  \
, BOOST_ACT_INTERLOCKED_IS_INTERLOCKED_INTEGER_DETAIL_ELEM                     \
, BOOST_ACT_INTERLOCKED_IS_INTERLOCKED_INTEGER_DETAIL_ELEM_DUMMY               \
)                                                                              \
( size )

namespace boost { namespace act { namespace interlocked {

namespace detail
{

typedef mpl::vector
        < mpl::void_
          BOOST_ACT_INTERLOCKED_IS_INTERLOCKED_INTEGER_DETAIL_ELEM_IF( 8 )
          BOOST_ACT_INTERLOCKED_IS_INTERLOCKED_INTEGER_DETAIL_ELEM_IF( 16 )
          BOOST_ACT_INTERLOCKED_IS_INTERLOCKED_INTEGER_DETAIL_ELEM_IF( 32 )
          BOOST_ACT_INTERLOCKED_IS_INTERLOCKED_INTEGER_DETAIL_ELEM_IF( 64 )
        >
        interlocked_integer_sequence;

#undef BOOST_ACT_INTERLOCKED_IS_INTERLOCKED_INTEGER_DETAIL_ELEM_IF
#undef BOOST_ACT_INTERLOCKED_IS_INTERLOCKED_INTEGER_DETAIL_ELEM_DUMMY
#undef BOOST_ACT_INTERLOCKED_IS_INTERLOCKED_INTEGER_DETAIL_ELEM

}

template< typename Type >
struct is_interlocked_integer
  : mpl::contains< detail::interlocked_integer_sequence, Type > {};

} } }

#else

#include <boost/mpl/bool.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename Type > struct is_interlocked_integer : mpl::false_ {};

} } }

#endif


#endif
