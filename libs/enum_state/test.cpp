// Test program to verify smart_enum<> and friends.

//////////////////////////////////////////////////
// Test program.                                //
//////////////////////////////////////////////////

// Boost library headers.
// #include <boost/function.hpp>
// #include <boost/lambda/lambda.hpp>
// #include <boost/lambda/bind.hpp>
#include <boost/static_assert.hpp>
#define BOOST_INCLUDE_MAIN
#include <boost/test/test_tools.hpp>

// The class we're testing.
#include <boost/enum_state.hpp>

class sequential_enum_test
    {
  public:
    sequential_enum_test()
        {
        typedef sequential_smart_enum<myEnum, state1, state4> my_enum_t;
        my_enum_t e(state1);

        // Test the boundaries
        BOOST_STATIC_ASSERT((myEnum(my_enum_t::min) == state1));
        BOOST_STATIC_ASSERT((myEnum(my_enum_t::max) == state4));

        // Test operator++
        BOOST_CRITICAL_TEST(e == state1);
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
        }
  private:
    enum myEnum { state1, state2, state3, state4 };
    };

class wrapped_enum_test
    {
  public:
    wrapped_enum_test()
        {
        typedef wrapped_smart_enum<myEnum, state1, state4> my_enum_t;
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
        }
  private:
    enum myEnum { state1 = 5, state2, state3, state4 };
    };

class bounded_enum_test
    {
  public:
    bounded_enum_test()
        {
        typedef bounded_smart_enum<myEnum, state1, state4> my_enum_t;
        my_enum_t e(state1);

        // Test the boundaries
        BOOST_STATIC_ASSERT((myEnum(my_enum_t::min) == state1));
        BOOST_STATIC_ASSERT((myEnum(my_enum_t::max) == state4));

        // Verify upper bound
        BOOST_CRITICAL_TEST(e == state1);
        BOOST_CRITICAL_TEST(++e == state2);
        BOOST_CRITICAL_TEST(++e == state3);
        BOOST_CRITICAL_TEST(++e == state4);
        BOOST_CRITICAL_TEST(++e == state4);

        // Verify lower bound
        BOOST_CRITICAL_TEST(--e == state3);
        BOOST_CRITICAL_TEST(--e == state2);
        BOOST_CRITICAL_TEST(--e == state1);
        BOOST_CRITICAL_TEST(--e == state1);

        // Verify assignment
        BOOST_CRITICAL_TEST((e = 50) == state4);
        BOOST_CRITICAL_TEST((e = -50) == state1);
        }
  private:
    enum myEnum { state1 = 5, state2, state3, state4 };
    };

class lambda_enum_test
    {
  public:
    lambda_enum_test()
        {
        }
  private:
    enum myEnum { north = 17, east = 4, south = 92, west = -5 };
    };

int test_main(int, char*[])
    {
    sequential_enum_test();
    wrapped_enum_test();
    bounded_enum_test();
    lambda_enum_test();
    return 0;
    }
