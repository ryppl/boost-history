// Test program to verify smart_enum<> and friends.

// Boost library headers.
#include <boost/static_assert.hpp>
#define BOOST_INCLUDE_MAIN
#include <boost/test/test_tools.hpp>

// The class we're testing.
#include <boost/smart_enum.hpp>

enum myEnum { state1 = 5, state2, state3, state4 };

int test_main(int, char*[])
    {
    typedef boost::wrapped_smart_enum<myEnum, state1, state4> my_enum_t;
    my_enum_t e(state1);

    // Test the boundaries
    BOOST_STATIC_ASSERT((myEnum(my_enum_t::min) == state1));
    BOOST_STATIC_ASSERT((myEnum(my_enum_t::max) == state4));

    // Verify wrapping properties
    BOOST_CRITICAL_TEST(e == state1);
    BOOST_CRITICAL_TEST(++e == state2);
    BOOST_CRITICAL_TEST(++e == state3);
    BOOST_CRITICAL_TEST(e++ == state3);
    BOOST_CRITICAL_TEST(e == state4);
    BOOST_CRITICAL_TEST(++e == state1);
    BOOST_CRITICAL_TEST((e += state4-state1+1) == state1);
    BOOST_CRITICAL_TEST((e = state3 + (state4-state1+1)*2) == state3);
    BOOST_CRITICAL_TEST((e += -10) == state1);

    // Done
    return 0;
    }
