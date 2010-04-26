
#include <boost/detail/workaround.hpp>

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
#  pragma warn -8091 // supress warning in Boost.Test
#  pragma warn -8057 // unused argument argc/argv in Boost.Test
#endif

#include <iostream> //apparently needed
#include <boost/test/test_tools.hpp>
#define BOOST_ASSIGN_CHECK_EQUAL(a,b) BOOST_CHECK_EQUAL(a,b)
#include <boost/assign/auto_size/check/ref_list_of.hpp>
#include <boost/test/unit_test.hpp>

#include <boost/assign/auto_size/check/unit_testing.hpp>
#define BOOST_ASSIGN_AS_CHECK_infix ref_list_of
   BOOST_ASSIGN_AS_CHECK_TEST_SUITE
#undef BOOST_ASSIGN_AS_CHECK_infix

#undef BOOST_ASSIGN_CHECK_EQUAL

