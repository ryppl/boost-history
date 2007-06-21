/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_TEST_INTERLOCKED_SUBTRACT_ASSIGN_SUBTRACT_ASSIGN_HELPER
#define BOOST_ACT_TEST_INTERLOCKED_SUBTRACT_ASSIGN_SUBTRACT_ASSIGN_HELPER

#include "../integral_additive_helper.hpp"
#include "../operation_result_checker.hpp"
#include <boost/act/interlocked/subtract_assign.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/preprocessor/cat.hpp>

struct BOOST_PP_CAT( interlocked_
                   , BOOST_ACT_TEST_INTERLOCKED_NAME
                   )
{
  template< typename LeftType, typename RightType >
  typename boost::remove_cv< LeftType >::type
  operator ()( LeftType& left, RightType const& right ) const
  {
    return boost::act::interlocked::BOOST_ACT_TEST_INTERLOCKED_NAME
           ( left, right );
  }
};

int test_main( int, char *[] )
{
  brute_operation_result_checker( basic_subtract()
                                , BOOST_PP_CAT( interlocked_
                                              , BOOST_ACT_TEST_INTERLOCKED_NAME
                                              )
                                              ()
                                );

  brute_additive_assign_test( basic_subtract_assign()
                            , BOOST_PP_CAT( interlocked_
                                          , BOOST_ACT_TEST_INTERLOCKED_NAME
                                          )
                                          ()
                            );

  return 0;
}

#endif
