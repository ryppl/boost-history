/*=============================================================================
    Spirit v1.7.0
    Copyright (c) 2003 Vaclav Vesely
    http://spirit.sourceforge.net/

    Permission to copy, use, modify, sell and distribute this software is
    granted provided this copyright notice appears in all copies. This
    software is provided "as is" without express or implied warranty, and
    with no claim as to its suitability for any purpose.
=============================================================================*/
#include <boost/spirit/core.hpp>
#include <boost/spirit/dynamic/lazy.hpp>
#include <boost/spirit/phoenix/primitives.hpp>
#include <boost/spirit/phoenix/operators.hpp>
#include <boost/test/included/unit_test_framework.hpp>

using namespace boost;
using namespace unit_test_framework;
using namespace spirit;
using namespace phoenix;

//-----------------------------------------------------------------------------

void lazy_p_test()
{
    int result;
    BOOST_CHECK(parse("123", lazy_p(val(int_p))[assign(result)]).full);
    BOOST_CHECK_EQUAL(result, 123);
}

//-----------------------------------------------------------------------------

test_suite* init_unit_test_suite(int argc, char* argv[])
{
    test_suite* test = BOOST_TEST_SUITE("lazy test");

    test->add(BOOST_TEST_CASE(&lazy_p_test));

    return test;
}

//-----------------------------------------------------------------------------
