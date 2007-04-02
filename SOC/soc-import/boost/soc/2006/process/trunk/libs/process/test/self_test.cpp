//
// Boost.Process
// Regression tests for the self class.
//
// Copyright (c) 2006, 2007 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/self.hpp>
#include <boost/test/unit_test.hpp>

namespace bp = ::boost::process;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

class launcher
{
public:
    bp::self
    operator()(bp::self::id_type id)
    {
        return bp::self();
    }
};

// ------------------------------------------------------------------------

static
void
test_id(void)
{
    bp::self p;

#if defined(BOOST_PROCESS_POSIX_API)
    BOOST_REQUIRE(p.get_id() == ::getpid());
#elif defined(BOOST_PROCESS_POSIX_API)
    BOOST_REQUIRE(p.get_id() == ::GetCurrentProcessId());
#endif
}

// ------------------------------------------------------------------------

static
void
test_get_environment(void)
{
    bp::environment env1 = bp::self().get_environment();
    BOOST_CHECK(env1.find("THIS_SHOULD_NOT_BE_DEFINED") == env1.end());

#if defined(BOOST_PROCESS_POSIX_API)
    BOOST_REQUIRE(::setenv("THIS_SHOULD_BE_DEFINED", "some-value", 1) == 0);
#elif defined(BOOST_PROCESS_WIN32_API)
    BOOST_REQUIRE(::SetEnvironmentVariable(TEXT("THIS_SHOULD_BE_DEFINED"),
                                           TEXT("some-value")) != 0);
#endif

    bp::environment env2 = bp::self().get_environment();
    bp::environment::const_iterator iter =
        env2.find("THIS_SHOULD_BE_DEFINED");
    BOOST_CHECK(iter != env2.end());
    BOOST_CHECK_EQUAL((*iter).second, "some-value");
}

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    but::test_suite* test = BOOST_TEST_SUITE("self test suite");

    test->add(BOOST_TEST_CASE(&test_id));
    test->add(BOOST_TEST_CASE(&test_get_environment));

    return test;
}
