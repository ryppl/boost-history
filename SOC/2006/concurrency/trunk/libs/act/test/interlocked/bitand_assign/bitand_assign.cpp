/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/test/minimal.hpp>
#include "../operation_result_checker.hpp"
#include <boost/act/interlocked/bitand_assign.hpp>
#include <boost/act/interlocked/bitand_assign/bitand_assign_result.hpp>

struct interlocked_bitand_assign
{
  template< typename LeftType, typename RightType >
  typename boost::act::interlocked::bitand_assign_result< LeftType >::type
  operator ()( LeftType& left, RightType const& right ) const
  {
    return boost::act::interlocked::bitand_assign( left, right );
  }
};

int test_main( int, char *[] )
{
  brute_operation_result_checker( basic_bitand()
                                , interlocked_bitand_assign()
                                );
/*
  brute_additive_assign_test( basic_add_assign()
                            , BOOST_PP_CAT( interlocked_
                                          , BOOST_ACT_TEST_INTERLOCKED_NAME
                                          )
                                          ()
                            );
*/
  return 0;
}
