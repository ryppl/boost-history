//
// Boost.Process
// Regression tests for the basic_process class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/basic_process.hpp>
#include <boost/test/unit_test.hpp>

namespace bp = ::boost::process;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

template< class Command_Line, class Attributes >
class test_process :
    public bp::basic_process< Command_Line, Attributes >
{
public:
    test_process(typename
                 test_process< Command_Line, Attributes >::handle_type h,
                 const Command_Line& cl,
                 const Attributes& a) :
        bp::basic_process< Command_Line, Attributes >(h, cl, a)
    {
    }
};

// ------------------------------------------------------------------------

static void
test_getters(void)
{
    test_process< char, int >::handle_type h1 =
        (test_process< char, int >::handle_type)0;
    char cl1 = 'A';
    int attrs1 = 5;
    test_process< char, int > p1(h1, cl1, attrs1);
    BOOST_CHECK_EQUAL(p1.get_handle(), h1);
    BOOST_CHECK_EQUAL(p1.get_command_line(), cl1);
    BOOST_CHECK_EQUAL(p1.get_attributes(), attrs1);

    test_process< char, int >::handle_type h2 =
        (test_process< char, int >::handle_type)1;
    char cl2 = 'B';
    int attrs2 = 6;
    test_process< char, int > p2(h2, cl2, attrs2);
    BOOST_CHECK_EQUAL(p2.get_handle(), h2);
    BOOST_CHECK_EQUAL(p2.get_command_line(), cl2);
    BOOST_CHECK_EQUAL(p2.get_attributes(), attrs2);
}

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    but::test_suite* test = BOOST_TEST_SUITE("basic_process test suite");

    test->add(BOOST_TEST_CASE(&test_getters));

    return test;
}
