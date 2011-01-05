/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#define BOOST_TEST_MODULE icl::interval_set_infix unit test
#include <libs/icl/test/disable_test_warnings.hpp>
#include <string>
#include <boost/mpl/list.hpp>
#include <libs/icl/test/unit_test_unwarned.hpp>
#include <boost/test/test_case_template.hpp>

// interval instance types
#include <libs/icl/test/test_type_lists.hpp>
#include <libs/icl/test/test_value_maker.hpp>

#include <boost/icl_xt/interval_bitset.hpp>

using namespace std;
using namespace boost;
using namespace unit_test;
using namespace boost::icl;

// -----------------------------------------------------------------------------
// test_interval_set_shared are tests that should give identical results for all
// interval_sets: interval_set, separate_interval_set and split_interval_set.
#define ICL_INTERVAL_BITSET_IMPL // Pass the interval_bitset template parameter
                                 // to the test functions via portability.hpp
#include <libs/icl/test/test_interval_set_shared.hpp>

#define INTERVAL_SET interval_bitset
#include <libs/icl_xt/test/fastest_interval_bitset_infix_cases.hpp>



