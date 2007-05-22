// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

$template_start$
$template_cpp_copyright$

#include <boost/test/unit_test.hpp>
using boost::unit_test::test_suite;

#include <boost/$template_library$.hpp>

class test_class
{
public:
    void test_nothing()
    {
        boost::$template_library$::$template_library$<int> object;
        int x = 0;
        BOOST_CHECK_EQUAL(object.get_nothing_count(),0);
        object.nothing(x);
        BOOST_CHECK_EQUAL(object.get_nothing_count(),1);
    }
    void test_nothing_n()
    {
        boost::$template_library$::$template_library$<int> object;
        int x = 0;
        BOOST_CHECK_EQUAL(object.get_call_count(),0);
        object.nothing(x,4);
        BOOST_CHECK_EQUAL(object.get_call_count(),1);
        BOOST_CHECK_EQUAL(object.get_nothing_count(),4);
    }
};

test_suite *$template_library$_test()
{
  test_suite *suite = BOOST_TEST_SUITE("$template_library$ suite");

  boost::shared_ptr<test_class> instance( new test_class() );
  suite->add (BOOST_CLASS_TEST_CASE( &test_class::test_nothing, instance));
  suite->add (BOOST_CLASS_TEST_CASE( &test_class::test_nothing_n, instance));
  return suite;
}

#define BOOST_TEST_MAIN
boost::unit_test::test_suite* init_unit_test_suite(int, char* [] )
{
  test_suite * test = BOOST_TEST_SUITE("_Boost_Library_ test");
  test->add($template_library$_test());
  return test;
}