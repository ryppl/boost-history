//
// Boost.Process
// Regression tests for the child class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/child.hpp>
#include <boost/process/detail/factories.hpp>
#include <boost/process/detail/pipe.hpp>
#include <boost/test/unit_test.hpp>

namespace bp = ::boost::process;
namespace bpd = ::boost::process::detail;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

static
void
test_getters(void)
{
    bp::child::handle_type h = (bp::child::handle_type)0;
    bpd::file_handle fhinvalid;
    bp::child c = bp::detail::factories::create_child
        (h, fhinvalid, fhinvalid, fhinvalid);

    BOOST_CHECK_EQUAL(c.get_handle(), h);
}

// ------------------------------------------------------------------------

static
void
test_stdin(void)
{
    bp::child::handle_type h = (bp::child::handle_type)0;
    bpd::pipe p;
    bpd::file_handle fhinvalid;
    bp::child c = bp::detail::factories::create_child
        (h, p.wend(), fhinvalid, fhinvalid);

    bp::postream& os = c.get_stdin();
    os << "test-stdin" << std::endl;

    bp::pistream is(p.rend());
    std::string word;
    is >> word;
    BOOST_CHECK_EQUAL(word, "test-stdin");
}

// ------------------------------------------------------------------------

static
void
test_stdout(void)
{
    bp::child::handle_type h = (bp::child::handle_type)0;
    bpd::pipe p;
    bpd::file_handle fhinvalid;
    bp::child c = bp::detail::factories::create_child
        (h, fhinvalid, p.rend(), fhinvalid);

    bp::postream os(p.wend());
    os << "test-stdout" << std::endl;

    bp::pistream& is = c.get_stdout();
    std::string word;
    is >> word;
    BOOST_CHECK_EQUAL(word, "test-stdout");
}

// ------------------------------------------------------------------------

static
void
test_stderr(void)
{
    bp::child::handle_type h = (bp::child::handle_type)0;
    bpd::pipe p;
    bpd::file_handle fhinvalid;
    bp::child c = bp::detail::factories::create_child
        (h, fhinvalid, fhinvalid, p.rend());

    bp::postream os(p.wend());
    os << "test-stderr" << std::endl;

    bp::pistream& is = c.get_stderr();
    std::string word;
    is >> word;
    BOOST_CHECK_EQUAL(word, "test-stderr");
}

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    but::test_suite* test = BOOST_TEST_SUITE("child test suite");

    test->add(BOOST_TEST_CASE(&test_getters));
    test->add(BOOST_TEST_CASE(&test_stdin));
    test->add(BOOST_TEST_CASE(&test_stdout));
    test->add(BOOST_TEST_CASE(&test_stderr));

    return test;
}
