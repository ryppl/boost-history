//
// Boost.Process
// Regression tests for the detail::systembuf class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#include <istream>
#include <fstream>
#include <ostream>

#include <boost/process/detail/systembuf.hpp>
#include <boost/test/unit_test.hpp>

#if defined(BOOST_PROCESS_WIN32_API)
#   include <windows.h>
#else
#   include <sys/stat.h>
#   include <fcntl.h>
#   include <unistd.h>
#endif

namespace bp = ::boost::process;
namespace bpd = ::boost::process::detail;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

static void
check_data(std::istream& is, size_t length)
{
    char ch = 'A', chr;
    size_t cnt = 0;
    while (is >> chr) {
        BOOST_CHECK(ch == chr);
        if (ch == 'Z')
            ch = 'A';
        else
            ch++;
        cnt++;
    }
    BOOST_CHECK(cnt == length);
}

// ------------------------------------------------------------------------

static void
write_data(std::ostream& os, size_t length)
{
    char ch = 'A';
    for (size_t i = 0; i < length; i++) {
        os << ch;
        if (ch == 'Z')
            ch = 'A';
        else
            ch++;
    }
    os.flush();
}

// ------------------------------------------------------------------------

static void
remove_file(const std::string& name)
{
#if defined(BOOST_PROCESS_WIN32_API)
#   error "Unimplemented."
#else
    ::unlink(name.c_str());
#endif
}

// ------------------------------------------------------------------------

static void
test_read(size_t length, size_t bufsize)
{
    std::ofstream f("test_read.txt");
    write_data(f, length);
    f.close();

#if defined(BOOST_PROCESS_WIN32_API)
#   error "Unimplemented."
#else
    int fd = ::open("test_read.txt", O_RDONLY);
    BOOST_CHECK(fd != -1);
    bpd::systembuf sb(fd, bufsize);
#endif
    std::istream is(&sb);
    check_data(is, length);
    ::close(fd);
    remove_file("test_read.txt");
}

// ------------------------------------------------------------------------

static void
test_short_read(void)
{
    test_read(64, 1024);
}

// ------------------------------------------------------------------------

static void
test_long_read(void)
{
    test_read(64 * 1024, 1024);
}

// ------------------------------------------------------------------------

static void
test_write(size_t length, size_t bufsize)
{
#if defined(BOOST_PROCESS_WIN32_API)
#   error "Unimplemented."
#else
    int fd = ::open("test_write.txt", O_WRONLY | O_CREAT | O_TRUNC,
                    S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    BOOST_CHECK(fd != -1);
    bpd::systembuf sb(fd, bufsize);
#endif
    std::ostream os(&sb);
    write_data(os, length);
    ::close(fd);

    std::ifstream is("test_write.txt");
    check_data(is, length);
    remove_file("test_write.txt");
}

// ------------------------------------------------------------------------

static void
test_short_write(void)
{
    test_write(64, 1024);
}

// ------------------------------------------------------------------------

static void
test_long_write(void)
{
    test_write(64 * 1024, 1024);
}

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    but::test_suite* test = BOOST_TEST_SUITE("detail::systembuf test suite");

    test->add(BOOST_TEST_CASE(&test_short_read));
    test->add(BOOST_TEST_CASE(&test_long_read));
    test->add(BOOST_TEST_CASE(&test_short_write));
    test->add(BOOST_TEST_CASE(&test_long_write));

    return test;
}
