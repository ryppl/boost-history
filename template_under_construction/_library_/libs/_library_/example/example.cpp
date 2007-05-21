_cpp_copyright_

//[ _library__example
#include <boost/test/unit_test.hpp>
using boost::unit_test::test_suite;

#include <boost/_library_.hpp>

void _library__example()
{
    boost::_library_::_library_<int> object;
    int x = 0;

    BOOST_CHECK_EQUAL(object.get_nothing_count(),0);
    object.nothing(x);
    BOOST_CHECK_EQUAL(object.get_nothing_count(),1);

    BOOST_CHECK_EQUAL(object.get_call_count(),1);
    object.nothing(x,4);
    BOOST_CHECK_EQUAL(object.get_call_count(),2);
    BOOST_CHECK_EQUAL(object.get_nothing_count(),5);

} // end void _library__example

test_suite* init_unit_test_suite(int argc, char* argv[])
{
    test_suite* test = BOOST_TEST_SUITE( "RPC test suite" );
    test->add(BOOST_TEST_CASE(&_library__example));
    return test;
}

//]