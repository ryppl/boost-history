// Test program to verify smart_enum<> and friends.

// Boost library headers.
#include <boost/static_assert.hpp>
#define BOOST_INCLUDE_MAIN
#include <boost/test/test_tools.hpp>

// The class we're testing.
#include <boost/smart_enum.hpp>

enum myEnum { state1, state2, state3, state4 };

int test_main(int, char*[])
    {
    typedef boost::sequential_smart_enum<myEnum, state1, state4> my_enum_t;
    my_enum_t e(state1);

    // Test the boundaries
    BOOST_STATIC_ASSERT((myEnum(my_enum_t::min) == state1));
    BOOST_STATIC_ASSERT((myEnum(my_enum_t::max) == state4));

    // Test operator++
    BOOST_CRITICAL_TEST(e == state1);
    BOOST_CRITICAL_TEST(e == 0);
    BOOST_CRITICAL_TEST(++e == state2);
    BOOST_CRITICAL_TEST(++e == state3);
    BOOST_CRITICAL_TEST(e++ == state3);
    BOOST_CRITICAL_TEST(e == state4);
    try { ++e; BOOST_CRITICAL_TEST(false); }
    catch(const std::out_of_range&) { }
    BOOST_CRITICAL_TEST(e == state4);

    // Test assignment
    try { e = 50; BOOST_CRITICAL_TEST(false); }
    catch(const std::out_of_range&) { }
    BOOST_CRITICAL_TEST(e == state4);
    BOOST_CRITICAL_TEST((e = state4) == state4);

    // Test operator--
    BOOST_CRITICAL_TEST(e-- == state4);
    BOOST_CRITICAL_TEST(e == state3);
    BOOST_CRITICAL_TEST(--e == state2);
    BOOST_CRITICAL_TEST(--e == state1);
    try { --e; BOOST_CRITICAL_TEST(false); }
    catch(const std::out_of_range&) { }
    BOOST_CRITICAL_TEST(e == state1);

    // Test operator+= and operator-=
    BOOST_CRITICAL_TEST((e += 1) == state2);
    try { e -= 5; BOOST_CRITICAL_TEST(false); }
    catch(const std::out_of_range&) { }
    BOOST_CRITICAL_TEST(e == state2);
    BOOST_CRITICAL_TEST((e -= 1) == state1);
    BOOST_CRITICAL_TEST((e -= -1) == state2);

    // Test free operator+ and operator-
    BOOST_CRITICAL_TEST((e = state1 + 1) == state2);
    try { e = e - 80; BOOST_CRITICAL_TEST(false); }
    catch(const std::out_of_range&) { }
    BOOST_CRITICAL_TEST(e == state2);
    try { e = -4 - 80; BOOST_CRITICAL_TEST(false); }
    catch(const std::out_of_range&) { }
    BOOST_CRITICAL_TEST(e == state2);
    BOOST_CRITICAL_TEST((e = 84 - 81) == state4);

    // Done
    return 0;
    }
