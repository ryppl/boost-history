//
// Boost.Process
// Regression tests for the environment class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_WIN32_API)
extern "C" {
#   include <windows.h>
}
#elif defined(BOOST_PROCESS_POSIX_API)
#   include <cstdlib>
#else
#   error "Unsupported platform."
#endif

#include <boost/process/detail/environment.hpp>
#include <boost/test/unit_test.hpp>

namespace bp = ::boost::process;
namespace bpd = ::boost::process::detail;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

bool
aux_env_is_set(const std::string& var)
{
    const char* val = ::getenv(var.c_str());
    return val != NULL;
}

// ------------------------------------------------------------------------

const std::string
aux_env_get(const std::string& var)
{
    const char* val = ::getenv(var.c_str());
    BOOST_REQUIRE(val != NULL);
    return val;
}

// ------------------------------------------------------------------------

void
aux_env_set(const std::string& var, const std::string& value)
{
    int err = ::setenv(var.c_str(), value.c_str(), 1);
    BOOST_REQUIRE(err != -1);
    BOOST_REQUIRE(aux_env_is_set(var));
}

// ------------------------------------------------------------------------

static void
test_set(void)
{
    bpd::environment env;
    BOOST_REQUIRE_EQUAL(env.size(),
                        static_cast<bpd::environment::size_type>(0));
    env.set("SOME_VARIABLE", "content");
    BOOST_REQUIRE_EQUAL(env.size(),
                        static_cast<bpd::environment::size_type>(1));
    bpd::environment::const_iterator iter = env.find("SOME_VARIABLE");
    BOOST_REQUIRE(iter != env.end());
    BOOST_REQUIRE_EQUAL((*iter).first, "SOME_VARIABLE");
    BOOST_REQUIRE((*iter).second.is_set());
    BOOST_REQUIRE_EQUAL((*iter).second.get_value(), "content");
}

// ------------------------------------------------------------------------

static void
test_unset(void)
{
    bpd::environment env;
    BOOST_REQUIRE_EQUAL(env.size(),
                        static_cast<bpd::environment::size_type>(0));
    env.unset("SOME_VARIABLE");
    BOOST_REQUIRE_EQUAL(env.size(),
                        static_cast<bpd::environment::size_type>(1));
    bpd::environment::const_iterator iter = env.find("SOME_VARIABLE");
    BOOST_REQUIRE(iter != env.end());
    BOOST_REQUIRE_EQUAL((*iter).first, "SOME_VARIABLE");
    BOOST_REQUIRE(!(*iter).second.is_set());
}

// ------------------------------------------------------------------------

static void
test_setup_set(void)
{
    BOOST_REQUIRE(!aux_env_is_set("TO_BE_SET"));

    bpd::environment env;
    env.set("TO_BE_SET", "some-value");
    env.setup();

    BOOST_REQUIRE(aux_env_is_set("TO_BE_SET"));
    BOOST_REQUIRE_EQUAL(aux_env_get("TO_BE_SET"), "some-value");
}

// ------------------------------------------------------------------------

static void
test_setup_unset(void)
{
    aux_env_set("TO_BE_UNSET", "some-value");

    bpd::environment env;
    env.unset("TO_BE_UNSET");
    env.setup();

    BOOST_REQUIRE(!aux_env_is_set("TO_BE_UNSET"));
}

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    but::test_suite* test = BOOST_TEST_SUITE("environment test suite");

    test->add(BOOST_TEST_CASE(&test_set));
    test->add(BOOST_TEST_CASE(&test_unset));
    test->add(BOOST_TEST_CASE(&test_setup_set));
    test->add(BOOST_TEST_CASE(&test_setup_unset));

    return test;
}
