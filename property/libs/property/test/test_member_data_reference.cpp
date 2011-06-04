#include <boost/test/unit_test.hpp>
#include <boost/property/property_member_data_reference.hpp>

void test_member_data_reference_function()
  {
  
  }

boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    boost::unit_test::test_suite* test = BOOST_TEST_SUITE( "Property Test Member Data Reference Suite" );

    test->add( BOOST_TEST_CASE( &test_member_data_reference_function ) );

    return test;
}
