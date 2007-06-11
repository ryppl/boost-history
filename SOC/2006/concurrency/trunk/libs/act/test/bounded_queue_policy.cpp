/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/detail/lightweight_test.hpp>

#include <boost/act/queue_policy/bounded_queue_policy.hpp>

#include "queue_policy_tester.hpp"

int main()
{
  test_queue_policy< boost::act::bounded_queue_policy<> >();

  return boost::report_errors();
}
