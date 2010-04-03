/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#define BOOST_TEST_MODULE itl::interval unit test
#include <string>
#include <boost/mpl/list.hpp>
#include "../unit_test_unwarned.hpp"
#include <boost/test/test_case_template.hpp>

// interval instance types
#include "../test_type_lists.hpp"
#include "../test_value_maker.hpp"

#include <boost/itl/continuous_interval.hpp>

using namespace std;
using namespace boost;
using namespace unit_test;
using namespace boost::itl;


#include "../test_itl_interval_shared.hpp"
//CL #include "../test_itl_interval.hpp"

#define     ORDERED_TYPES             continuous_types
#define BICREMENTAL_TYPES bicremental_continuous_types

#define INTERVAL continuous_interval
#include "../test_interval_cases.hpp" //Common cases for different interval types

