/*=============================================================================
    Copyright (c) 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_HAS_INT_HPP
#define BOOST_ACT_INTERLOCKED_HAS_INT_HPP

#include <boost/act/interlocked/detail/cas_support.hpp>

#ifdef BOOST_ACT_INTERLOCKED_DETAIL_OPERATION_SUPPORT

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/logical/bitor.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/control/if.hpp>

#include <boost/preprocessor/comparison/greater_equal.hpp>
#include <boost/preprocessor/logical/bitor.hpp>
#include <boost/preprocessor/seq/filter.hpp>
#include <boost/preprocessor/seq/size.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_HAS_INT_LEAST_PREDICATE( s,num_bits,elem )\
BOOST_PP_BITOR                                                                 \
( BOOST_PP_EQUAL( elem, 0 )                                                    \
, BOOST_PP_GREATER_EQUAL( elem, num_bits )                                     \
)

#define BOOST_ACT_INTERLOCKED_DETAIL_HAS_INT_PREDICATE( s,num_bits,elem )      \
BOOST_PP_BITOR                                                                 \
( BOOST_PP_EQUAL( elem, 0 )                                                    \
, BOOST_PP_EQUAL( elem, num_bits )                                             \
)

#define BOOST_ACT_INTERLOCKED_HAS_INT_LEAST( num_bits )                        \
BOOST_PP_GREATER_EQUAL                                                         \
( BOOST_PP_SEQ_SIZE                                                            \
  ( BOOST_PP_SEQ_FILTER( BOOST_ACT_INTERLOCKED_DETAIL_HAS_INT_LEAST_PREDICATE  \
                       , num_bits                                              \
                       , BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_SEQ(0)       \
                       )                                                       \
  )                                                                            \
, 2                                                                            \
)

#define BOOST_ACT_INTERLOCKED_HAS_UINT_LEAST( num_bits )                       \
BOOST_ACT_INTERLOCKED_HAS_INT_LEAST( num_bits )

#define BOOST_ACT_INTERLOCKED_HAS_INT( num_bits )                              \
BOOST_PP_GREATER_EQUAL                                                         \
( BOOST_PP_SEQ_SIZE                                                            \
  ( BOOST_PP_SEQ_FILTER( BOOST_ACT_INTERLOCKED_DETAIL_HAS_INT_PREDICATE        \
                       , num_bits                                              \
                       , BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_SEQ(0)       \
                       )                                                       \
  )                                                                            \
, 2                                                                            \
)

#define BOOST_ACT_INTERLOCKED_HAS_UINT( num_bits )                             \
BOOST_ACT_INTERLOCKED_HAS_INT_LEAST( num_bits )

#else

#define BOOST_ACT_INTERLOCKED_HAS_INT_LEAST( num_bits )  0
#define BOOST_ACT_INTERLOCKED_HAS_UINT_LEAST( num_bits ) 0

#define BOOST_ACT_INTERLOCKED_HAS_INT( num_bits )  0
#define BOOST_ACT_INTERLOCKED_HAS_UINT( num_bits ) 0

#endif

#endif
