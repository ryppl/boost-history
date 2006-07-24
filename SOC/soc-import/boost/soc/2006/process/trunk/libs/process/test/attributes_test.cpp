//
// Boost.Process
// Regression tests for the attributes class.
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
extern "C" {
#   include <unistd.h>
}
#else
#   error "Unsupported platform."
#endif

#include <string>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/process/attributes.hpp>
#include <boost/test/unit_test.hpp>

namespace bfs = ::boost::filesystem;
namespace bp = ::boost::process;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

namespace boost {
namespace process {

class launcher {
public:
    void setup(const attributes& a)
    {
        a.setup();
    }
};

} // namespace process
} // namespace boost

// ------------------------------------------------------------------------

static void
test_default_work_directory(void)
{
    bp::attributes a;
    BOOST_CHECK(bfs::equivalent(a.get_work_directory(),
                                bfs::current_path().string()));
}

// ------------------------------------------------------------------------

static void
test_explicit_work_directory(void)
{
    bp::attributes a("/work/directory");
    BOOST_CHECK_EQUAL(a.get_work_directory(), "/work/directory");
}

// ------------------------------------------------------------------------

static void
test_setup(void)
{
    bp::attributes a1("non-existent");
    BOOST_CHECK_THROW(bp::launcher().setup(a1), bp::system_error);

    bfs::path testdir = bfs::current_path() / "test-dir";
    BOOST_REQUIRE(bfs::create_directory(testdir));
    try {
        bp::attributes a2(testdir.string());
        bp::launcher().setup(a2);
        BOOST_CHECK_EQUAL(bfs::current_path(), testdir);

        const char* initial = bfs::initial_path().string().c_str();
#if defined(BOOST_PROCESS_WIN32_API)
        BOOST_CHECK(::SetCurrentDirectory(TEXT(initial)) != 0);
#elif defined(BOOST_PROCESS_POSIX_API)
        BOOST_CHECK(::chdir(initial) != -1);
#endif

        bfs::remove(testdir);
    } catch (...) {
        bfs::remove(testdir);
        throw;
    }
}

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    bfs::initial_path();

    but::test_suite* test = BOOST_TEST_SUITE("attributes test suite");

    test->add(BOOST_TEST_CASE(&test_default_work_directory));
    test->add(BOOST_TEST_CASE(&test_explicit_work_directory));
    test->add(BOOST_TEST_CASE(&test_setup));

    return test;
}
