/* Copyright (C) 2001
 * Housemarque Oy
 * http://www.housemarque.com
 *
 * Permission to copy, use, modify, sell and distribute this software is
 * granted provided this copyright notice appears in all copies. This
 * software is provided "as is" without express or implied warranty, and
 * with no claim as to its suitability for any purpose.
 *
 * See http://www.boost.org for most recent version.
 */

/* This example implements a couple of 1-dimensional arithmetic array
 * manipulation functions in C. The idea is to use preprocessor data
 * structures, lists and tuples, for storing meta information to be used
 * for generating the actual C code.
 *
 * Compile with gcc. It will take a while!
 */

#include <libs/preprocessor/example/array_arithmetic_helpers.hpp>
#include <boost/preprocessor/list/at.hpp>
#include <boost/preprocessor/tuple/to_list.hpp>
#include <boost/preprocessor/tuple/eat.hpp>
#include <boost/preprocessor/comparison/less.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/comparison/not_equal.hpp>
#include <boost/preprocessor/logical/or.hpp>
#include <boost/preprocessor/logical/and.hpp>
#include <boost/preprocessor/arithmetic/mul.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <stddef.h>

  /*sym , name          ,bin,flt,log,sh?,com*/
#define OPS\
  (( !  , logical_not   , 0 , 1 , 1 , 0 , 0 ),\
  (( ~  , bitwise_not   , 0 , 0 , 0 , 0 , 0 ),\
  (( -  , neg           , 0 , 1 , 0 , 0 , 0 ),\
  (( *  , mul           , 1 , 1 , 0 , 0 , 1 ),\
  (( /  , div           , 1 , 1 , 0 , 0 , 0 ),\
  (( %  , mod           , 1 , 0 , 0 , 0 , 0 ),\
  (( +  , add           , 1 , 1 , 0 , 0 , 1 ),\
  (( -  , sub           , 1 , 1 , 0 , 0 , 0 ),\
  (( << , shift_left    , 1 , 0 , 0 , 1 , 0 ),\
  (( >> , shift_right   , 1 , 0 , 0 , 1 , 0 ),\
  (( <  , less          , 1 , 1 , 1 , 0 , 0 ),\
  (( <= , less_equal    , 1 , 1 , 1 , 0 , 0 ),\
  (( >= , greater_equal , 1 , 1 , 1 , 0 , 0 ),\
  (( >  , greater       , 1 , 1 , 1 , 0 , 0 ),\
  (( == , equal         , 1 , 1 , 1 , 0 , 1 ),\
  (( != , not_equal     , 1 , 1 , 1 , 0 , 1 ),\
  (( &  , bitwise_and   , 1 , 0 , 0 , 0 , 1 ),\
  (( |  , bitwise_or    , 1 , 0 , 0 , 0 , 1 ),\
  (( ^  , bitwise_xor   , 1 , 0 , 0 , 0 , 1 ),\
  (( && , logical_and   , 1 , 1 , 1 , 0 , 1 ),\
  (( || , logical_or    , 1 , 1 , 1 , 0 , 1 ),\
  (_,_,0),1),1),1),1),1),1),1),1),1),1),1),1),1),1),1),1),1),1),1),1),1)

#define OP_SYMBOL(O)         BOOST_PP_TUPLE7_ELEM0 O
#define OP_NAME(O)           BOOST_PP_TUPLE7_ELEM1 O
#define OP_IS_BINARY(O)      BOOST_PP_TUPLE7_ELEM2 O
#define OP_IS_FLOATING(O)    BOOST_PP_TUPLE7_ELEM3 O
#define OP_IS_LOGICAL(O)     BOOST_PP_TUPLE7_ELEM4 O
#define OP_IS_SHIFT(O)       BOOST_PP_TUPLE7_ELEM5 O
#define OP_IS_COMMUTATIVE(O) BOOST_PP_TUPLE7_ELEM6 O

  /* type           ,abbr,flt,lng,uns,siz*/
#define TYPES\
  (( signed char    , sc , 0 , 0 , 0 , 1 ),\
  (( unsigned char  , uc , 0 , 0 , 1 , 1 ),\
  (( short          , ss , 0 , 0 , 0 , 2 ),\
  (( unsigned short , us , 0 , 0 , 1 , 2 ),\
  (TYPE_INT,\
  (( unsigned int   , ui , 0 , 0 , 1 , 4 ),\
  (( long           , sl , 0 , 1 , 0 , 4 ),\
  (( unsigned long  , ul , 0 , 1 , 1 , 4 ),\
  (( float          , fl , 1 , 0 , 0 , 4 ),\
  (( double         , db , 1 , 0 , 0 , 8 ),\
  (( long double    , ld , 1 , 1 , 0 , 8 ),\
   (_,_,0),1),1),1),1),1),1),1),1),1),1),1)

#define TYPE_INT    ( int , si , 0 , 0 , 0 , 4 )

#define TYPE_NAME(T)         BOOST_PP_TUPLE6_ELEM0 T
#define TYPE_ABBREVIATION(T) BOOST_PP_TUPLE6_ELEM1 T
#define TYPE_IS_FLOATING(T)  BOOST_PP_TUPLE6_ELEM2 T
#define TYPE_IS_LONG(T)      BOOST_PP_TUPLE6_ELEM3 T
#define TYPE_IS_UNSIGNED(T)  BOOST_PP_TUPLE6_ELEM4 T
#define TYPE_SIZE(T)         BOOST_PP_TUPLE6_ELEM5 T
#define TYPE_RANK(T)\
  BOOST_PP_ADD\
  ( BOOST_PP_ADD\
    ( BOOST_PP_ADD\
      ( BOOST_PP_MUL(64,TYPE_IS_FLOATING(T))\
      , BOOST_PP_MUL(32,TYPE_IS_LONG(T))\
      )\
    , BOOST_PP_MUL(TYPE_SIZE(T),2)\
    )\
  , TYPE_IS_UNSIGNED(T)\
  )
#define TYPE_IS_SAME(L,R) BOOST_PP_EQUAL(TYPE_RANK(L),TYPE_RANK(R))

#define INTEGER_PROMOTION(T) BOOST_PP_IF(BOOST_PP_LESS(TYPE_RANK(T),TYPE_RANK(TYPE_INT)),TYPE_INT,T)
#define UNARY_OP_RESULT(O,T) BOOST_PP_IF(OP_IS_LOGICAL(O),TYPE_INT,INTEGER_PROMOTION(T))
#define USUAL_ARITHMETIC_CONVERSION(L,R) INTEGER_PROMOTION(BOOST_PP_IF(BOOST_PP_LESS(TYPE_RANK(L),TYPE_RANK(R)),R,L))
#define BINARY_OP_RESULT(O,L,R)\
  BOOST_PP_IF\
  ( OP_IS_LOGICAL(O)\
  , TYPE_INT\
  , BOOST_PP_IF\
    ( OP_IS_SHIFT(O)\
    , INTEGER_PROMOTION(L)\
    , USUAL_ARITHMETIC_CONVERSION(L,R)\
    )\
  )

#define INSTANTIATE(_,L)\
  INSTANTIATE_HELPER(BOOST_PP_LIST_AT(L,2),BOOST_PP_LIST_AT(L,1),BOOST_PP_LIST_AT(L,0))
#define INSTANTIATE_HELPER(O,L,R)\
  BOOST_PP_IF(OP_IS_BINARY(O),INSTANTIATE_BINARY,INSTANTIATE_UNARY)(O,L,R)

#define INSTANTIATE_UNARY(O,L,R)\
  BOOST_PP_IF\
  ( BOOST_PP_AND\
    ( TYPE_IS_SAME(L,R)\
    , BOOST_PP_NOT_EQUAL(BOOST_PP_ADD(BOOST_PP_MUL(2,TYPE_IS_FLOATING(L)),OP_IS_FLOATING(O)),2)\
    )\
  , INSTANTIATE_UNARY_HELPER\
  , BOOST_PP_TUPLE_EAT(2)\
  )(O,L)
#define INSTANTIATE_UNARY_HELPER(O,T)\
  void BOOST_PP_CAT4(array_,OP_NAME(O),_,TYPE_ABBREVIATION(T))\
    (const TYPE_NAME(T)* in, TYPE_NAME(UNARY_OP_RESULT(O,T))* out, size_t n)\
  { do { *out++ = OP_SYMBOL(O) *in++; } while (--n); }

#define INSTANTIATE_BINARY(O,L,R)\
  BOOST_PP_IF\
  ( BOOST_PP_NOT_EQUAL(BOOST_PP_ADD(BOOST_PP_MUL(2,BOOST_PP_OR(TYPE_IS_FLOATING(L),TYPE_IS_FLOATING(R))),OP_IS_FLOATING(O)),2)\
  , INSTANTIATE_BINARY_HELPER\
  , BOOST_PP_TUPLE_EAT(3)\
  )(O,L,R)
#define INSTANTIATE_BINARY_HELPER(O,L,R)\
  void BOOST_PP_CAT6(array_,OP_NAME(O),_,TYPE_ABBREVIATION(L),_,TYPE_ABBREVIATION(R))\
  (const TYPE_NAME(L)* lhs_in, const TYPE_NAME(R)* rhs_in, TYPE_NAME(BINARY_OP_RESULT(O,L,R))* out, size_t n)\
  { do { *out++ = *lhs_in++ OP_SYMBOL(O) *rhs_in++; } while (--n); }

BOOST_PP_LIST_FOR_EACH_PRODUCT(INSTANTIATE,_,BOOST_PP_TUPLE_TO_LIST(3,(OPS,TYPES,TYPES)))
