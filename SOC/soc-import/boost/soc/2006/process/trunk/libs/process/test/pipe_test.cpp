//
// Boost.Process
// Regression tests for the detail::pipe class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#include <istream>
#include <ostream>

#include <boost/process/detail/pipe.hpp>
#include <boost/process/detail/systembuf.hpp>
#include <boost/test/unit_test.hpp>

namespace bp = ::boost::process;
namespace bpd = ::boost::process::detail;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

static void
test_read_and_write(void)
{
    try {
        bpd::pipe p;
        bpd::systembuf rbuf(p.get_read_end());
        bpd::systembuf wbuf(p.get_write_end());
        std::istream rend(&rbuf);
        std::ostream wend(&wbuf);

        // XXX This assumes that the pipe's buffer is big enough to accept
        // the data written without blocking!
        wend << "Test message" << std::endl;
        std::string tmp;
        rend >> tmp;
        BOOST_CHECK(tmp == "Test");
        rend >> tmp;
        BOOST_CHECK(tmp == "message");
    } catch (const bp::system_error& e) {
        // XXX Any better way to handle exceptions in tests?
        BOOST_CHECK(false);
    }
}

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    but::test_suite* test = BOOST_TEST_SUITE("detail::pipe test suite");

    test->add(BOOST_TEST_CASE(&test_read_and_write));

    return test;
}
