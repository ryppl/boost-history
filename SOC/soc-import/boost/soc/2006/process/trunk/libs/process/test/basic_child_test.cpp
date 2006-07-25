//
// Boost.Process
// Regression tests for the basic_child class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/basic_child.hpp>
#include <boost/process/detail/pipe.hpp>
#include <boost/test/unit_test.hpp>

namespace bp = ::boost::process;
namespace bpd = ::boost::process::detail;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

namespace boost {
namespace process {

class launcher
{
public:
    template< class Command_Line, class Attributes >
    static
    bp::basic_child< Command_Line, Attributes > create_child
        (typename bp::basic_child< Command_Line, Attributes >::handle_type h,
         const Command_Line& cl,
         const Attributes& attrs,
         detail::file_handle& fhstdin,
         detail::file_handle& fhstdout,
         detail::file_handle& fhstderr)
    {
        return basic_child< Command_Line, Attributes >
            (h, cl, attrs, fhstdin, fhstdout, fhstderr);
    }
};

} // namespace boost
} // namespace process

// ------------------------------------------------------------------------

static
void
test_getters(void)
{
    bp::basic_child< char, int >::handle_type h =
        (bp::basic_child< char, int >::handle_type)0;
    char cl = 'A';
    int attrs = 5;
    bpd::file_handle fhinvalid;
    bp::basic_child< char, int > c =
        bp::launcher::create_child(h, cl, attrs,
                                   fhinvalid, fhinvalid, fhinvalid);

    BOOST_CHECK_EQUAL(c.get_handle(), h);
    BOOST_CHECK_EQUAL(c.get_command_line(), cl);
    BOOST_CHECK_EQUAL(c.get_attributes(), attrs);
}

// ------------------------------------------------------------------------

static
void
test_stdin(void)
{
    bp::basic_child< char, int >::handle_type h =
        (bp::basic_child< char, int >::handle_type)0;
    char cl = 'A';
    int attrs = 5;
    bpd::pipe p;
    bpd::file_handle fhinvalid;
    bp::basic_child< char, int > c =
        bp::launcher::create_child(h, cl, attrs,
                                   p.wend(), fhinvalid, fhinvalid);

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
    bp::basic_child< char, int >::handle_type h =
        (bp::basic_child< char, int >::handle_type)0;
    char cl = 'A';
    int attrs = 5;
    bpd::pipe p;
    bpd::file_handle fhinvalid;
    bp::basic_child< char, int > c =
        bp::launcher::create_child(h, cl, attrs,
                                   fhinvalid, p.rend(), fhinvalid);

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
    bp::basic_child< char, int >::handle_type h =
        (bp::basic_child< char, int >::handle_type)0;
    char cl = 'A';
    int attrs = 5;
    bpd::pipe p;
    bpd::file_handle fhinvalid;
    bp::basic_child< char, int > c =
        bp::launcher::create_child(h, cl, attrs,
                                   fhinvalid, fhinvalid, p.rend());

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
    but::test_suite* test = BOOST_TEST_SUITE("basic_child test suite");

    test->add(BOOST_TEST_CASE(&test_getters));
    test->add(BOOST_TEST_CASE(&test_stdin));
    test->add(BOOST_TEST_CASE(&test_stdout));
    test->add(BOOST_TEST_CASE(&test_stderr));

    return test;
}
