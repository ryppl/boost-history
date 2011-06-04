#include <boost/test/unit_test.hpp>
#include <boost/property/property_function_reference.hpp>

void test_function_reference_function()
  {
  
  }

boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    boost::unit_test::test_suite* test = BOOST_TEST_SUITE( "Property Test Function Reference Suite" );

    test->add( BOOST_TEST_CASE( &test_function_reference_function ) );

    return test;
}
