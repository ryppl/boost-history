//
// Boost.Process
// Regression tests for the child class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// This code may be used under either of the following two licences:
//
//      Permission is hereby granted, free of charge, to any person
//      obtaining a copy of this software and associated documentation
//      files (the "Software"), to deal in the Software without
//      restriction, including without limitation the rights to use,
//      copy, modify, merge, publish, distribute, sublicense, and/or
//      sell copies of the Software, and to permit persons to whom the
//      Software is furnished to do so, subject to the following
//      conditions:
//
//      The above copyright notice and this permission notice shall be
//      included in all copies or substantial portions of the Software.
//
//      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//      EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//      OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//      NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//      HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//      WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//      FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//      OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
// Or:
//
//      Distributed under the Boost Software License, Version 1.0.
//      (See accompanying file LICENSE_1_0.txt or copy at
//      http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/process/detail/pipe.hpp>
#include <boost/test/unit_test.hpp>

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
// Start concept
// -------------
//
// The functions in this file all rely on a Factory concept.  This concept
// provides a class whose () operator constructs a new Child instance
// based on a process handle and three file handles, one for each of the
// standard communication channels.  Note that this is the most possible
// generic construction, which should be conceptually supported by all
// implementations.
//

// ------------------------------------------------------------------------

template< class Child, class Factory >
static
void
test_getters(void)
{
    typename Child::handle_type h = (typename Child::handle_type)0;
    bpd::file_handle fhinvalid;
    Child c = Factory()(h, fhinvalid, fhinvalid, fhinvalid);

    BOOST_CHECK_EQUAL(c.get_handle(), h);
}

// ------------------------------------------------------------------------

template< class Child, class Factory >
static
void
test_stdin(void)
{
    typename Child::handle_type h = (typename Child::handle_type)0;
    bpd::pipe p;
    bpd::file_handle fhinvalid;
    Child c = Factory()(h, p.wend(), fhinvalid, fhinvalid);

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
    typename Child::handle_type h = (typename Child::handle_type)0;
    bpd::pipe p;
    bpd::file_handle fhinvalid;
    Child c = Factory()(h, fhinvalid, p.rend(), fhinvalid);

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
    typename Child::handle_type h = (typename Child::handle_type)0;
    bpd::pipe p;
    bpd::file_handle fhinvalid;
    Child c = Factory()(h, fhinvalid, fhinvalid, p.rend());

    bp::postream os(p.wend());
    os << "test-stderr" << std::endl;

    bp::pistream& is = c.get_stderr();
    std::string word;
    is >> word;
    BOOST_CHECK_EQUAL(word, "test-stderr");
}

// ------------------------------------------------------------------------

} // namespace child_base_test

// ------------------------------------------------------------------------

template< class Child, class Factory >
void
add_tests_child_base(boost::unit_test::test_suite* ts)
{
    using namespace child_base_test;

    ts->add(BOOST_TEST_CASE(&(test_getters< Child, Factory >)));
    ts->add(BOOST_TEST_CASE(&(test_stdin< Child, Factory >)));
    ts->add(BOOST_TEST_CASE(&(test_stdout< Child, Factory >)));
    ts->add(BOOST_TEST_CASE(&(test_stderr< Child, Factory >)));
}
