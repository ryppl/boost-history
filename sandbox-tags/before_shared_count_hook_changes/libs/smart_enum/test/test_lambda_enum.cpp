// Test program to verify smart_enum<> and friends.

// Boost library headers.
#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/if.hpp>
#include <boost/lambda/switch.hpp>
#include <boost/lambda/loops.hpp>
#include <boost/lambda/casts.hpp>
#include <boost/lambda/numeric.hpp>
#include <boost/lambda/exceptions.hpp>
#include <boost/static_assert.hpp>
#define BOOST_INCLUDE_MAIN
#include <boost/test/test_tools.hpp>

// The class we're testing.
#include <boost/smart_enum.hpp>

enum myEnum { north = 17, east = 4, south = 92, west = -5 };

// There will to be some serious magic going on here soon. Trust me:
// The following "using" statement should make you nervous.

using namespace boost::lambda;

int test_main(int, char*[])
    {
    typedef boost::smart_enum<myEnum, boost::function<myEnum, int, int> > my_enum_t;
    my_enum_t e(north, (
        // Verify that _1 is a valid myEnum.

        if_then(_1 != north && _1 != east && _1 != south && _1 != west,
                throw_exception(std::out_of_range("invalid myEnum!"))),

        // Our enum is wrapped, what allows for two nice short-cuts.

        _2 = _2 % 4,
        if_then(_2 < 0, _2 = 4 + _2),

        // Now increment appropriately.

        while_loop(--_2 >= 0, switch_statement(
                       _1,
                       case_statement<north>(_1 = east),
                       case_statement<east> (_1 = south),
                       case_statement<south>(_1 = west),
                       case_statement<west> (_1 = north))),

        // Return result.

        ll_static_cast<myEnum>(_1)));

    // Test operator++
    BOOST_CRITICAL_TEST(e == north);
    BOOST_CRITICAL_TEST(++e == east);
    BOOST_CRITICAL_TEST(++e == south);
    BOOST_CRITICAL_TEST(++e == west);
    BOOST_CRITICAL_TEST(++e == north);
    BOOST_CRITICAL_TEST(++e == east);

    // Test operator--
    BOOST_CRITICAL_TEST(--e == north);
    BOOST_CRITICAL_TEST(--e == west);
    BOOST_CRITICAL_TEST(--e == south);
    BOOST_CRITICAL_TEST(--e == east);
    BOOST_CRITICAL_TEST(--e == north);

    // Test operator+= and operator-=
    BOOST_CRITICAL_TEST((e += 1) == east);
    BOOST_CRITICAL_TEST((e -= 2) == west);
    BOOST_CRITICAL_TEST((e -= 8) == west);
    BOOST_CRITICAL_TEST((e += 7) == south);

    // Test assignment
    BOOST_CRITICAL_TEST((e = -5) == west);
    try { e = 0; BOOST_CRITICAL_TEST(false); }
    catch(const std::out_of_range&) { }
    BOOST_CRITICAL_TEST(e == west);
    BOOST_CRITICAL_TEST((e = 80 + 12) == south);
    BOOST_CRITICAL_TEST((e = e / 23) == east);

    // Done
    return 0;
    }
