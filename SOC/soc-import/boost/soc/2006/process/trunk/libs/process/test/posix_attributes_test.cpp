//
// Boost.Process
// Regression tests for the posix_attributes class.
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt.)
//

#include <boost/process/config.hpp>

#if defined(BOOST_PROCESS_POSIX_API)
extern "C" {
#   include <sys/types.h>
#   include <unistd.h>
}

#   include <string>

#   include <boost/filesystem/path.hpp>
#   include <boost/process/posix_attributes.hpp>

namespace bp = ::boost::process;
#endif

#include <boost/filesystem/operations.hpp>
#include <boost/test/unit_test.hpp>

namespace bfs = ::boost::filesystem;
namespace but = ::boost::unit_test;

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
namespace boost {
namespace process {

class launcher {
public:
    static void setup(const posix_attributes& pa)
    {
        pa.setup();
    }
};

} // namespace process
} // namespace boost
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static void
test_default_ids(void)
{
    bp::posix_attributes pa;
    BOOST_CHECK_EQUAL(pa.get_gid(), ::getgid());
    BOOST_CHECK_EQUAL(pa.get_egid(), ::getegid());
    BOOST_CHECK_EQUAL(pa.get_uid(), ::getuid());
    BOOST_CHECK_EQUAL(pa.get_euid(), ::geteuid());
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static void
test_default_setup(void)
{
    gid_t gid = ::getgid();
    gid_t egid = ::getegid();
    uid_t uid = ::getuid();
    uid_t euid = ::geteuid();
    bfs::path p = bfs::current_path();

    bp::posix_attributes pa;
    BOOST_CHECK_NO_THROW(bp::launcher::setup(pa));

    BOOST_CHECK_EQUAL(gid, ::getgid());
    BOOST_CHECK_EQUAL(egid, ::getegid());
    BOOST_CHECK_EQUAL(uid, ::getuid());
    BOOST_CHECK_EQUAL(euid, ::geteuid());
    BOOST_CHECK_EQUAL(p, bfs::current_path());
}
#endif

// ------------------------------------------------------------------------

#if defined(BOOST_PROCESS_POSIX_API)
static void
test_setters(void)
{
    bp::posix_attributes pa1;
    gid_t gid = pa1.get_gid() + 1;
    pa1.set_gid(gid);
    BOOST_CHECK_EQUAL(pa1.get_gid(), gid);

    bp::posix_attributes pa2;
    gid_t egid = pa2.get_egid() + 1;
    pa2.set_egid(egid);
    BOOST_CHECK_EQUAL(pa2.get_egid(), egid);

    bp::posix_attributes pa3;
    uid_t uid = pa3.get_uid() + 1;
    pa3.set_uid(uid);
    BOOST_CHECK_EQUAL(pa3.get_uid(), uid);

    bp::posix_attributes pa4;
    uid_t euid = pa4.get_euid() + 1;
    pa4.set_euid(euid);
    BOOST_CHECK_EQUAL(pa4.get_euid(), euid);

    bp::posix_attributes pa5;
    pa5.set_chroot("/some/directory");
    BOOST_CHECK_EQUAL(pa5.get_chroot(), "/some/directory");
}
#endif

// ------------------------------------------------------------------------

#if !defined(BOOST_PROCESS_POSIX_API)
static
void
test_dummy(void)
{
}
#endif

// ------------------------------------------------------------------------

but::test_suite *
init_unit_test_suite(int argc, char* argv[])
{
    bfs::initial_path();

    but::test_suite* test = BOOST_TEST_SUITE("posix_attributes test suite");

#if defined(BOOST_PROCESS_POSIX_API)
    test->add(BOOST_TEST_CASE(&test_default_ids));
    test->add(BOOST_TEST_CASE(&test_default_setup));
    test->add(BOOST_TEST_CASE(&test_setters));

    // TODO: Check that posix_attributes::setup() works.  This is quite
    // difficult because it needs root privileges and because it needs to
    // know about an existing user account and group on the system to do
    // the switch.  Not to mention how complex it can be to test the
    // chroot feature...
    //
    // Furthermore, this check probably needs to go in its own file
    // because, to be safe, it will need to use the launcher to spawn the
    // modified "environment".
#else
    test->add(BOOST_TEST_CASE(&test_dummy));
#endif

    return test;
}
