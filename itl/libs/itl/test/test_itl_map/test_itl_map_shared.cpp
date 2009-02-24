/*-----------------------------------------------------------------------------+
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_itl_map_find_4_bicremental_types, T, bicremental_types)
{         itl_map_find_4_bicremental_types<T, int, partial_absorber, interval_map>();}

