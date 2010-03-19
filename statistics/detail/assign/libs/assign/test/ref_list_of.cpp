
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
using boost::unit_test::test_suite;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test = BOOST_TEST_SUITE( "List Test Suite" );
    using namespace boost::assign::detail::auto_size;
    test->add( BOOST_TEST_CASE( &check_ref_list_of_int ) );

    return test;
}


#undef BOOST_ASSIGN_CHECK_EQUAL


