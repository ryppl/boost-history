//
// Boost.Process
// Regression tests for Child implementations.
//
// Copyright (c) 2006, 2007 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/detail/pipe.hpp>
#include <boost/test/unit_test.hpp>

#include "process_base_test.hpp"

namespace bp = ::boost::process;
namespace bpd = ::boost::process::detail;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

namespace child_base_test {

// ------------------------------------------------------------------------

//
// Overview
// --------
//
// The functions below implement tests for the basic Child implementation.
// In order to ensure appropriate behavior, all implementations must
// have the same behavior in common public methods; keeping this set of
// tests generic makes it easy to check this restriction because the tests
// can easily be applied to any specific Child implementation.
//
// Factory concept
// ---------------
//
// The functions in this file all rely on a Factory concept.  This concept
// provides a class whose () operator constructs a new Child instance
// based on a process's identifier and three file handles, one for each of
// the standard communication channels.  Note that this is the most possible
// generic construction, which should be conceptually supported by all
// implementations.
//

// ------------------------------------------------------------------------

template< class Child, class Factory >
static
void
test_stdin(void)
{
    typename Child::id_type id = (typename Child::id_type)0;
    bpd::pipe p;
    bpd::file_handle fhinvalid;
    Child c = Factory()(id, p.wend(), fhinvalid, fhinvalid);

    bp::postream& os = c.get_stdin();
    os << "test-stdin" << std::endl;

    bp::pistream is(p.rend());
    std::string word;
    is >> word;
    BOOST_CHECK_EQUAL(word, "test-stdin");
}

// ------------------------------------------------------------------------

template< class Child, class Factory >
static
void
test_stdout(void)
{
    typename Child::id_type id = (typename Child::id_type)0;
    bpd::pipe p;
    bpd::file_handle fhinvalid;
    Child c = Factory()(id, fhinvalid, p.rend(), fhinvalid);

    bp::postream os(p.wend());
    os << "test-stdout" << std::endl;

    bp::pistream& is = c.get_stdout();
    std::string word;
    is >> word;
    BOOST_CHECK_EQUAL(word, "test-stdout");
}

// ------------------------------------------------------------------------

template< class Child, class Factory >
static
void
test_stderr(void)
{
    typename Child::id_type id = (typename Child::id_type)0;
    bpd::pipe p;
    bpd::file_handle fhinvalid;
    Child c = Factory()(id, fhinvalid, fhinvalid, p.rend());

    bp::postream os(p.wend());
    os << "test-stderr" << std::endl;

    bp::pistream& is = c.get_stderr();
    std::string word;
    is >> word;
    BOOST_CHECK_EQUAL(word, "test-stderr");
}

// ------------------------------------------------------------------------

template< class Child >
class launcher
{
public:
    Child
    operator()(typename Child::id_type id)
    {
        bpd::file_handle fhinvalid;
        return Child(id, fhinvalid, fhinvalid, fhinvalid);
    }
};

// ------------------------------------------------------------------------

} // namespace child_base_test

// ------------------------------------------------------------------------

template< class Child, class Factory >
void
add_tests_child_base(boost::unit_test::test_suite* ts)
{
    using namespace child_base_test;
    using namespace process_base_test;

    add_tests_process_base< Child, launcher< Child > >(ts);

    ts->add(BOOST_TEST_CASE(&(test_stdin< Child, Factory >)));
    ts->add(BOOST_TEST_CASE(&(test_stdout< Child, Factory >)));
    ts->add(BOOST_TEST_CASE(&(test_stderr< Child, Factory >)));
}
