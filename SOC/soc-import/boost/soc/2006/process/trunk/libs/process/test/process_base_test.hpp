//
// Boost.Process
// Regression tests for Process implementations.
//
// Copyright (c) 2006, 2007 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/child.hpp>
#include <boost/process/context.hpp>
#include <boost/process/detail/pipe.hpp>
#include <boost/process/operations.hpp>
#include <boost/process/status.hpp>
#include <boost/process/posix_status.hpp>
#include <boost/test/unit_test.hpp>

#include "misc.hpp"

namespace bp = ::boost::process;
namespace bpd = ::boost::process::detail;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

namespace process_base_test {

// ------------------------------------------------------------------------

//
// Overview
// --------
//
// The functions below implement tests for the basic Process implementation.
// In order to ensure appropriate behavior, all implementations must
// have the same behavior in common public methods; keeping this set of
// tests generic makes it easy to check this restriction because the tests
// can easily be applied to any specific Process implementation.
//
// Factory concept
// ---------------
//
// The functions in this file all rely on a Factory concept.  This concept
// provides a class whose () operator constructs a new Process instance
// based on a process's identifier.  Note that this is the most possible
// generic construction, which should be conceptually supported by all
// implementations.
//

// ------------------------------------------------------------------------

template< class Process, class Factory >
static
void
test_getters(void)
{
    typename Process::id_type id = (typename Process::id_type)0;
    Process c = Factory()(id);

    BOOST_CHECK_EQUAL(c.get_id(), id);
}

// ------------------------------------------------------------------------

template< class Process, class Factory >
static
void
test_terminate(void)
{
    std::vector< std::string > args;
    args.push_back("helpers");
    args.push_back("loop");

    bp::context ctx;

    bp::child c = bp::launch(get_helpers_path(), args, ctx);

    Process p = Factory()(c.get_id());
    p.terminate();

    bp::status s = c.wait();
    if (bp_api_type == posix_api) {
        BOOST_REQUIRE(!s.exited());

        bp::posix_status ps = s;
        BOOST_REQUIRE(ps.signaled());
    } else if (bp_api_type == win32_api) {
        BOOST_REQUIRE(s.exited());
        BOOST_REQUIRE(s.exit_status() == EXIT_FAILURE);
    }
}

// ------------------------------------------------------------------------

} // namespace process_base_test

// ------------------------------------------------------------------------

template< class Process, class Factory >
void
add_tests_process_base(boost::unit_test::test_suite* ts)
{
    using namespace process_base_test;

    ts->add(BOOST_TEST_CASE(&(test_getters< Process, Factory >)));
    ts->add(BOOST_TEST_CASE(&(test_terminate< Process, Factory >)));
}
