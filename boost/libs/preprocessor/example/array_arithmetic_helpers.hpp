#ifndef BOOST_LIBS_PREPROCESSOR_EXAMPLE_ARRAY_ARITHMETIC_HELPERS_HPP
#define BOOST_LIBS_PREPROCESSOR_EXAMPLE_ARRAY_ARITHMETIC_HELPERS_HPP

/* Copyright (C) 2002
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

#include <boost/preprocessor/list/adt.hpp>
#include <boost/preprocessor/for.hpp>
#include <boost/preprocessor/cat.hpp>

#define BOOST_PP_CAT3(A,B,C) BOOST_PP_CAT(BOOST_PP_CAT(A,B),C)
#define BOOST_PP_CAT4(A,B,C,D) BOOST_PP_CAT3(BOOST_PP_CAT(A,B),C,D)
#define BOOST_PP_CAT5(A,B,C,D,E) BOOST_PP_CAT4(BOOST_PP_CAT(A,B),C,D,E)
#define BOOST_PP_CAT6(A,B,C,D,E,F) BOOST_PP_CAT5(BOOST_PP_CAT(A,B),C,D,E,F)

#define BOOST_PP_LIST_FOR_EACH_PRODUCT(F,P,LL) BOOST_PP_LIST_FOR_EACH_PRODUCT_R(0,F,P,LL)

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define BOOST_PP_LIST_FOR_EACH_PRODUCT_R(R,F,P,LL)\
  BOOST_PP_FOR##R\
  ( (BOOST_PP_TUPLE3_ELEM0 LL,BOOST_PP_TUPLE3_ELEM1 LL,(_,_,0),F,P)\
  , BOOST_PP_LIST_FOR_EACH_PRODUCT_C\
  , BOOST_PP_LIST_FOR_EACH_PRODUCT_F\
  , BOOST_PP_LIST_FOR_EACH_PRODUCT_I0\
  )
#define BOOST_PP_LIST_FOR_EACH_PRODUCT_C(R,P)\
  BOOST_PP_TUPLE3_ELEM2 BOOST_PP_TUPLE5_ELEM0 P
#define BOOST_PP_LIST_FOR_EACH_PRODUCT_F(R,P)\
  ( BOOST_PP_LIST_REST(BOOST_PP_TUPLE5_ELEM0 P)\
  , BOOST_PP_TUPLE5_ELEM1 P\
  , BOOST_PP_TUPLE5_ELEM2 P\
  , BOOST_PP_TUPLE5_ELEM3 P\
  , BOOST_PP_TUPLE5_ELEM4 P\
  )
#define BOOST_PP_LIST_FOR_EACH_PRODUCT_I_HELPER(P,C,F,I)\
  BOOST_PP_TUPLE5_ELEM3 P(BOOST_PP_TUPLE5_ELEM4 P,BOOST_PP_TUPLE5_ELEM2 P)
#define BOOST_PP_LIST_FOR_EACH_PRODUCT_I0(R,P)\
  BOOST_PP_IF\
  ( BOOST_PP_TUPLE3_ELEM2 BOOST_PP_TUPLE5_ELEM1 P\
  , BOOST_PP_FOR##R\
  , BOOST_PP_LIST_FOR_EACH_PRODUCT_I_HELPER\
  )\
   ( (BOOST_PP_TUPLE3_ELEM0 BOOST_PP_TUPLE5_ELEM1 P,BOOST_PP_TUPLE3_ELEM1 BOOST_PP_TUPLE5_ELEM1 P,(BOOST_PP_TUPLE3_ELEM0 BOOST_PP_TUPLE5_ELEM0 P,BOOST_PP_TUPLE5_ELEM2 P,1),BOOST_PP_TUPLE5_ELEM3 P,BOOST_PP_TUPLE5_ELEM4 P)\
   , BOOST_PP_LIST_FOR_EACH_PRODUCT_C\
   , BOOST_PP_LIST_FOR_EACH_PRODUCT_F\
   , BOOST_PP_LIST_FOR_EACH_PRODUCT_I##1\
   )
#define BOOST_PP_LIST_FOR_EACH_PRODUCT_I1(R,P)\
  BOOST_PP_IF\
  ( BOOST_PP_TUPLE3_ELEM2 BOOST_PP_TUPLE5_ELEM1 P\
  , BOOST_PP_FOR##R\
  , BOOST_PP_LIST_FOR_EACH_PRODUCT_I_HELPER\
  )\
   ( (BOOST_PP_TUPLE3_ELEM0 BOOST_PP_TUPLE5_ELEM1 P,BOOST_PP_TUPLE3_ELEM1 BOOST_PP_TUPLE5_ELEM1 P,(BOOST_PP_TUPLE3_ELEM0 BOOST_PP_TUPLE5_ELEM0 P,BOOST_PP_TUPLE5_ELEM2 P,1),BOOST_PP_TUPLE5_ELEM3 P,BOOST_PP_TUPLE5_ELEM4 P)\
   , BOOST_PP_LIST_FOR_EACH_PRODUCT_C\
   , BOOST_PP_LIST_FOR_EACH_PRODUCT_F\
   , BOOST_PP_LIST_FOR_EACH_PRODUCT_I##2\
   )
#define BOOST_PP_LIST_FOR_EACH_PRODUCT_I2(R,P)\
  BOOST_PP_IF\
  ( BOOST_PP_TUPLE3_ELEM2 BOOST_PP_TUPLE5_ELEM1 P\
  , BOOST_PP_FOR##R\
  , BOOST_PP_LIST_FOR_EACH_PRODUCT_I_HELPER\
  )\
   ( (BOOST_PP_TUPLE3_ELEM0 BOOST_PP_TUPLE5_ELEM1 P,BOOST_PP_TUPLE3_ELEM1 BOOST_PP_TUPLE5_ELEM1 P,(BOOST_PP_TUPLE3_ELEM0 BOOST_PP_TUPLE5_ELEM0 P,BOOST_PP_TUPLE5_ELEM2 P,1),BOOST_PP_TUPLE5_ELEM3 P,BOOST_PP_TUPLE5_ELEM4 P)\
   , BOOST_PP_LIST_FOR_EACH_PRODUCT_C\
   , BOOST_PP_LIST_FOR_EACH_PRODUCT_F\
   , BOOST_PP_LIST_FOR_EACH_PRODUCT_I##3\
   )
#define BOOST_PP_LIST_FOR_EACH_PRODUCT_I3(R,P)\
  BOOST_PP_IF\
  ( BOOST_PP_TUPLE3_ELEM2 BOOST_PP_TUPLE5_ELEM1 P\
  , BOOST_PP_FOR##R\
  , BOOST_PP_LIST_FOR_EACH_PRODUCT_I_HELPER\
  )\
   ( (BOOST_PP_TUPLE3_ELEM0 BOOST_PP_TUPLE5_ELEM1 P,BOOST_PP_TUPLE3_ELEM1 BOOST_PP_TUPLE5_ELEM1 P,(BOOST_PP_TUPLE3_ELEM0 BOOST_PP_TUPLE5_ELEM0 P,BOOST_PP_TUPLE5_ELEM2 P,1),BOOST_PP_TUPLE5_ELEM3 P,BOOST_PP_TUPLE5_ELEM4 P)\
   , BOOST_PP_LIST_FOR_EACH_PRODUCT_C\
   , BOOST_PP_LIST_FOR_EACH_PRODUCT_F\
   , BOOST_PP_LIST_FOR_EACH_PRODUCT_I##4\
   )

#endif
#endif
